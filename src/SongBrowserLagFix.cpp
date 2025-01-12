#include <Geode/Geode.hpp>
#include <Geode/modify/MusicDownloadManager.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/CustomSongCell.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include <Geode/modify/GJSongBrowser.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include <shared_mutex>

using namespace geode::prelude;

static inline std::unordered_map<int, bool> s_downloadedSongs;
static inline std::shared_mutex s_populating;
void populateDownloadedSongsFast() {
    auto MDM = MusicDownloadManager::sharedState();
    std::vector<int> knownSongs;
    auto dict = CCDictionaryExt<std::string, CCString*>(MDM->m_songObjects);
    for(auto [id, song] : dict) {
        if(auto result = utils::numFromString<int>(id)) {
            knownSongs.push_back(result.unwrap());
        }
    }

    auto songPath = GameManager::sharedState()->getGameVariable("0033") ? CCFileUtils::sharedFileUtils()->getWritablePath2() : CCFileUtils::sharedFileUtils()->getWritablePath();
    std::thread([knownSongs, songPath]() {
        thread::setName("Song Browser Lag Fix");

        std::unique_lock lock(s_populating);
        log::debug("Started populating downloaded songs cache");
        for(auto song : knownSongs) {
            if(std::filesystem::exists(fmt::format("{}/{}.mp3", std::string(songPath), song))) {
                s_downloadedSongs[song] = true;
            }
        }
        log::debug("Finished populating downloaded songs cache");
    }).detach();
}

void resetSongsCache() {
    std::unique_lock lock(s_populating);
    s_downloadedSongs.clear();
    populateDownloadedSongsFast();
}

class $modify(BIMusicDownloadManager, MusicDownloadManager) {
    bool isSongDownloaded(int songID) {
        std::shared_lock lock(s_populating);
        if(s_downloadedSongs.contains(songID)) {
            return s_downloadedSongs[songID];
        }
        lock.unlock();

        std::unique_lock uLock(s_populating);
        s_downloadedSongs[songID] = MusicDownloadManager::isSongDownloaded(songID);
        return s_downloadedSongs[songID];
    }

    void downloadSong(int songID) {
        std::unique_lock lock(s_populating);
        if(s_downloadedSongs.contains(songID)) s_downloadedSongs.erase(songID);
        lock.unlock();

        MusicDownloadManager::downloadSong(songID);
    }

    void downloadCustomSong(int songID) {
        std::unique_lock lock(s_populating);
        if(s_downloadedSongs.contains(songID)) s_downloadedSongs.erase(songID);
        lock.unlock();

        MusicDownloadManager::downloadCustomSong(songID);
    }

    void onDownloadSongCompleted(CCHttpClient* client, CCHttpResponse* response) {
        std::unique_lock lock(s_populating);
        auto songID = atoi(response->getHttpRequest()->getTag());
        if(s_downloadedSongs.contains(songID)) s_downloadedSongs[songID] = true;
        lock.unlock();

        MusicDownloadManager::onDownloadSongCompleted(client, response);
    }

    //TODO: onDownloadSongCompleted

    #ifndef GEODE_IS_WINDOWS
    // this function is inlined on windows

    void limitDownloadedSongs() {
        std::unique_lock lock(s_populating);
        s_downloadedSongs.clear();
        lock.unlock();

        MusicDownloadManager::limitDownloadedSongs();
    }

    void deleteSong(int songID) {
        MusicDownloadManager::deleteSong(songID);

        std::unique_lock lock(s_populating);
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
        std::unique_lock lock(s_populating);
        if(s_downloadedSongs.contains(this->m_level->m_songID)) s_downloadedSongs.erase(this->m_level->m_songID);
        lock.unlock();

        LevelInfoLayer::onPlay(sender);
    }

    bool init(GJGameLevel* level, bool challenge) {
        std::unique_lock lock(s_populating);
        if(s_downloadedSongs.contains(level->m_songID)) s_downloadedSongs.erase(level->m_songID);
        lock.unlock();

        return LevelInfoLayer::init(level, challenge);
    }
};

class $modify(CustomSongWidget) {
    void updateSongInfo() {
        std::unique_lock lock(s_populating);
        if(m_songInfoObject && s_downloadedSongs.contains(m_songInfoObject->m_songID)) s_downloadedSongs.erase(m_songInfoObject->m_songID);
        lock.unlock();

        CustomSongWidget::updateSongInfo();
    }

    void deleteSong() {
        auto id = m_songInfoObject ? m_songInfoObject->m_songID : 0;

        CustomSongWidget::deleteSong();

        std::unique_lock lock(s_populating);
        if(s_downloadedSongs.contains(id)) s_downloadedSongs.erase(id);
    }
};

// the button got deleted in 2.206
/*class $modify(CustomSongCell) {
    void onDelete(CCObject* sender) {
        auto id = m_songInfoObject ? m_songInfoObject->m_songID : 0;
        CustomSongCell::onDelete(sender);

        std::unique_lock lock(s_populating);
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