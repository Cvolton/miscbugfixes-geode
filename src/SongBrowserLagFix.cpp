#include <Geode/Geode.hpp>
#include <Geode/modify/MusicDownloadManager.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/CustomSongCell.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include <Geode/modify/GJSongBrowser.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

static inline std::unordered_map<int, bool> s_downloadedSongs;
void populateDownloadedSongsFast() {
    std::string songPath = GameManager::sharedState()->getGameVariable("0033") 
        ? CCFileUtils::sharedFileUtils()->getWritablePath2() 
        : CCFileUtils::sharedFileUtils()->getWritablePath();

    async::spawn([songPath = std::move(songPath)] mutable -> arc::Future<> {
        std::unordered_map<int, bool> downloadedSongs;

        std::error_code ec;
        log::debug("Started populating downloaded songs cache");
        for (const auto & entry : std::filesystem::directory_iterator(songPath, ec)) {
            if(entry.is_regular_file()) {
                auto filename = utils::string::pathToString(entry.path().filename());
                if(filename.size() > 4 && filename.ends_with(".mp3")) {
                    auto id = utils::numFromString<int>(filename.substr(0, filename.size() - 4));
                    if(id) downloadedSongs[id.unwrap()] = true;
                }
            }
        }

        co_await waitForMainThread([downloadedSongs = std::move(downloadedSongs)]() mutable {
            s_downloadedSongs = std::move(downloadedSongs);
        });
        log::debug("Finished populating downloaded songs cache");
    });
}

void resetSongsCache() {
    s_downloadedSongs.clear();
    populateDownloadedSongsFast();
}

class $modify(BIMusicDownloadManager, MusicDownloadManager) {
    bool isSongDownloaded(int songID) {
        if(s_downloadedSongs.contains(songID)) {
            return s_downloadedSongs[songID];
        }

        s_downloadedSongs[songID] = MusicDownloadManager::isSongDownloaded(songID);
        return s_downloadedSongs[songID];
    }

    void downloadSong(int songID) {
        if(s_downloadedSongs.contains(songID)) s_downloadedSongs.erase(songID);

        MusicDownloadManager::downloadSong(songID);
    }

    void downloadCustomSong(int songID) {
        if(s_downloadedSongs.contains(songID)) s_downloadedSongs.erase(songID);

        MusicDownloadManager::downloadCustomSong(songID);
    }

    void onDownloadSongCompleted(CCHttpClient* client, CCHttpResponse* response) {
        auto songID = atoi(response->getHttpRequest()->getTag());
        if(s_downloadedSongs.contains(songID)) s_downloadedSongs[songID] = true;

        MusicDownloadManager::onDownloadSongCompleted(client, response);
    }

    #ifndef GEODE_IS_WINDOWS
    // this function is inlined on windows

    void limitDownloadedSongs() {
        s_downloadedSongs.clear();

        MusicDownloadManager::limitDownloadedSongs();
    }

    void deleteSong(int songID) {
        MusicDownloadManager::deleteSong(songID);

        if(s_downloadedSongs.contains(songID)) s_downloadedSongs.erase(songID);
    }
    #endif
};

class $modify(GJSongBrowser) {
    void FLAlert_Clicked(FLAlertLayer* alert, bool btn2) {
        GJSongBrowser::FLAlert_Clicked(alert, btn2);

        resetSongsCache();
    }
};

class $modify(BISBLLevelInfoLayer, LevelInfoLayer) {
    void onPlay(CCObject* sender) {
        if(s_downloadedSongs.contains(this->m_level->m_songID)) s_downloadedSongs.erase(this->m_level->m_songID);

        LevelInfoLayer::onPlay(sender);
    }

    bool init(GJGameLevel* level, bool challenge) {
        if(s_downloadedSongs.contains(level->m_songID)) s_downloadedSongs.erase(level->m_songID);

        return LevelInfoLayer::init(level, challenge);
    }
};

class $modify(CustomSongWidget) {
    void updateSongInfo() {
        if(m_songInfoObject && s_downloadedSongs.contains(m_songInfoObject->m_songID)) s_downloadedSongs.erase(m_songInfoObject->m_songID);

        CustomSongWidget::updateSongInfo();
    }

    void deleteSong() {
        auto id = m_songInfoObject ? m_songInfoObject->m_songID : 0;

        CustomSongWidget::deleteSong();

        if(s_downloadedSongs.contains(id)) s_downloadedSongs.erase(id);
    }
};

// the button got deleted in 2.206
/*class $modify(CustomSongCell) {
    void onDelete(CCObject* sender) {
        auto id = m_songInfoObject ? m_songInfoObject->m_songID : 0;
        CustomSongCell::onDelete(sender);

        if(s_downloadedSongs.contains(id)) s_downloadedSongs.erase(id);
    }
};*/

#include "_MenuLayerManager.hpp"
$on_mod(Loaded) {
    MenuLayerManager::queueFunction([](MenuLayer*) {
        log::debug("Loading song browser lag fix");
        populateDownloadedSongsFast();
    });
}