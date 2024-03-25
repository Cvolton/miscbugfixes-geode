#include <Geode/Geode.hpp>
#include <stdio.h>

#ifdef GEODE_IS_WINDOWS
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

static bool s_patchChecked = false;

void safeRestart() {
    // since we rename the original executable,
    // geode updater is unable to see whether the game is still open
    // so we will save the game and then crash it

    AppDelegate::get()->trySaveGame(true);

    log::info("Game saved, restarting...");

    const auto workingDir = dirs::getGameDir();

    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    const auto gdName = fmt::format("\"{}\"", ghc::filesystem::path(buffer).filename().string());

    // launch updater
    const auto updaterPath = (workingDir / "GeodeUpdater.exe").string();
    ShellExecuteA(nullptr, "open", updaterPath.c_str(), gdName.c_str(), workingDir.string().c_str(), false);

    log::info("Updater launched, crashing game...");

    std::exit(0);
}

bool is4GBPatchEnabled() {
    wchar_t result[MAX_PATH];
    GetModuleFileNameW(NULL, result, MAX_PATH);

    if(!std::filesystem::exists(result)) {
        log::error("4 GB Patch: Unable to find executable.");
        return true; // just so the dialog doesn't pop up
    }
        
    std::ifstream file(result, std::ios::binary);
    std::vector<char> buffer(std::istreambuf_iterator<char>(file), {});

    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)buffer.data();
    PIMAGE_NT_HEADERS imageNTHeaders = (PIMAGE_NT_HEADERS)((DWORD)buffer.data() + dosHeader->e_lfanew);
    auto characteristics = imageNTHeaders->FileHeader.Characteristics;
    return characteristics & IMAGE_FILE_LARGE_ADDRESS_AWARE;
}

void showFailure() {
    auto popup = createQuickPopup(
        "Misc Bugfixes",
        "Failed to enable 4GB patch. You can try again later.",
        "OK",
        nullptr,
        nullptr,
        false
    );
    popup->show();
}

void showSuccess() {
    auto popup = createQuickPopup(
        "Misc Bugfixes",
        "<cy>4GB Patch</c> <cg>enabled</c>!\n<cj>Restart</c> your game to enjoy its benefits!",
        "Not Now",
        "Restart",
        [](auto alert, bool btn2) {
            if(btn2) {
                safeRestart();
            }
        },
        false
    );
    popup->show();
}

void enablePatch() {
    wchar_t result[MAX_PATH];
    GetModuleFileNameW(NULL, result, MAX_PATH);

    if(!std::filesystem::exists(result)) {
        log::error("4 GB Patch: Unable to find executable.");
        return;
    }

    std::ifstream file(result, std::ios::binary);
    std::vector<char> buffer(std::istreambuf_iterator<char>(file), {});

    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)buffer.data();
    PIMAGE_NT_HEADERS imageNTHeaders = (PIMAGE_NT_HEADERS)((DWORD)buffer.data() + dosHeader->e_lfanew);

    imageNTHeaders->FileHeader.Characteristics |= IMAGE_FILE_LARGE_ADDRESS_AWARE;
    file.close();

    //hacky workaround to avoid file in use error
    //ty prevter for the idea
    for(size_t i = 0; ; i++) {
        auto filename = std::wstring(result) + L".bak" + std::to_wstring(i);
        if(!std::filesystem::exists(filename)) {
            try {
                std::filesystem::rename(result, filename);
            } catch(const std::filesystem::filesystem_error& e) {
                log::error("4 GB Patch: Failed to rename exe - {}", e.what());
                return showFailure();
            }
            break;
        }
    }

    std::ofstream ofile(result, std::ios::binary);
    ofile.write(buffer.data(), buffer.size());
    ofile.close();

    log::info("4GB Patch enabled!");

    return showSuccess();
}

class $modify(MenuLayer) {
    bool init() {
        if(!MenuLayer::init()) return false;

        if(s_patchChecked) return true;
        if(Mod::get()->getSettingValue<bool>("skip-4gb-patch-check")) return true;

        std::thread([this]() {
            log::debug("Checking for 4GB patch...");
            if(is4GBPatchEnabled()) {
                log::debug("4GB patch is enabled.");
            } else {
                Loader::get()->queueInMainThread([this]() {
                    auto winSize = CCDirector::sharedDirector()->getWinSize();
                    auto enablePopup = createQuickPopup(
                        "Misc Bugfixes",
                        "<cy>4GB Patch</c> is <cr>disabled</c>! <cg>Enable</c> it?\nThis <cj>fixes</c> a significant amount of <co>crashes</c>!\n\n",
                        "No",
                        "Yes",
                        [this](auto alert, bool btn2) {
                            if(!btn2) {
                                log::info("User chose not to enable 4GB patch.");
                                auto dontEnablePopup = createQuickPopup(
                                    "Misc Bugfixes",
                                    "You can enable the 4GB patch later in the mod settings.",
                                    "OK",
                                    nullptr,
                                    nullptr,
                                    false
                                );
                                dontEnablePopup->m_scene = this;
                                dontEnablePopup->show();
                                Mod::get()->setSettingValue("skip-4gb-patch-check", true);
                            } else {
                                log::info("User chose to enable 4GB patch.");
                                enablePatch();
                            }
                        },
                        false
                    );
                    
                    auto bottomText = CCLabelBMFont::create("(This modifies GeometryDash.exe)", "chatFont.fnt");
                    bottomText->setScale(.6f);
                    bottomText->setZOrder(5);
                    bottomText->setPosition({winSize.width / 2, winSize.height / 2 - 23});
                    bottomText->setID("bottom-text"_spr);

                    enablePopup->m_mainLayer->addChild(bottomText);
                    enablePopup->m_scene = this;
                    enablePopup->show();
                });
            }
        }).detach();

        s_patchChecked = true;

        return true;
    }
};

#endif