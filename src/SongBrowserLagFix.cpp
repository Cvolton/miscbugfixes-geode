#include <Geode/Geode.hpp>
#include <Geode/modify/MusicDownloadManager.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/CustomSongCell.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include <Geode/modify/GJSongBrowser.hpp>
#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

static inline std::unordered_map<int, bool> s_downloadedSongs;
static inline std::atomic_bool s_populating = false;
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

        while(s_populating) { /* wait (spinlock) */}
        s_populating = true;
        log::debug("Started populating downloaded songs cache");
        for(auto song : knownSongs) {
            if(ghc::filesystem::exists(fmt::format("{}/{}.mp3", std::string(songPath), song))) {
                s_downloadedSongs[song] = true;
            }
        }
        log::debug("Finished populating downloaded songs cache");
        s_populating = false;
    }).detach();
}

void resetSongsCache() {
    while(s_populating) { /* wait (spinlock) */}
    s_downloadedSongs.clear();
    populateDownloadedSongsFast();
}

class $modify(BIMusicDownloadManager, MusicDownloadManager) {
    bool isSongDownloaded(int songID) {
        if(s_downloadedSongs.contains(songID)) {
            return s_downloadedSongs[songID];
        }

        while(s_populating) { /* wait (spinlock) */}
        s_downloadedSongs[songID] = MusicDownloadManager::isSongDownloaded(songID);
        return s_downloadedSongs[songID];
    }

    void downloadSong(int songID) {
        while(s_populating) { /* wait (spinlock) */}
        if(s_downloadedSongs.contains(songID)) s_downloadedSongs.erase(songID);

        MusicDownloadManager::downloadSong(songID);
    }

    void downloadCustomSong(int songID) {
        while(s_populating) { /* wait (spinlock) */}
        if(s_downloadedSongs.contains(songID)) s_downloadedSongs.erase(songID);

        MusicDownloadManager::downloadCustomSong(songID);
    }

    void limitDownloadedSongs() {
        while(s_populating) { /* wait (spinlock) */}
        s_downloadedSongs.clear();

        MusicDownloadManager::limitDownloadedSongs();
    }

    void onDownloadSongCompleted(CCHttpClient* client, CCHttpResponse* response) {
        while(s_populating) { /* wait (spinlock) */}
        auto songID = atoi(response->getHttpRequest()->getTag());
        if(s_downloadedSongs.contains(songID)) s_downloadedSongs[songID] = true;

        MusicDownloadManager::onDownloadSongCompleted(client, response);
    }

    #ifndef GEODE_IS_WINDOWS
    // this function is inlined on windows
    void deleteSong(int songID) {
        MusicDownloadManager::deleteSong(songID);

        while(s_populating) { /* wait (spinlock) */}
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
        while(s_populating) { /* wait (spinlock) */}
        if(s_downloadedSongs.contains(this->m_level->m_songID)) s_downloadedSongs.erase(this->m_level->m_songID);

        LevelInfoLayer::onPlay(sender);
    }

    bool init(GJGameLevel* level, bool challenge) {
        while(s_populating) { /* wait (spinlock) */}
        if(s_downloadedSongs.contains(level->m_songID)) s_downloadedSongs.erase(level->m_songID);

        return LevelInfoLayer::init(level, challenge);
    }
};

class $modify(CustomSongWidget) {
    void updateSongInfo() {
        while(s_populating) { /* wait (spinlock) */}
        if(m_songInfoObject && s_downloadedSongs.contains(m_songInfoObject->m_songID)) s_downloadedSongs.erase(m_songInfoObject->m_songID);

        CustomSongWidget::updateSongInfo();
    }

    void deleteSong() {
        auto id = m_songInfoObject ? m_songInfoObject->m_songID : 0;

        CustomSongWidget::deleteSong();

        while(s_populating) { /* wait (spinlock) */}
        if(s_downloadedSongs.contains(id)) s_downloadedSongs.erase(id);
    }
};

    class $modify(CustomSongCell) {
    void onDelete(CCObject* sender) {
        auto id = m_songInfoObject ? m_songInfoObject->m_songID : 0;
        CustomSongCell::onDelete(sender);

        while(s_populating) { /* wait (spinlock) */}
        if(s_downloadedSongs.contains(id)) s_downloadedSongs.erase(id);
    }
};

class $modify(MenuLayer) {
    bool init() {
        if(!MenuLayer::init()) return false;

        static bool loaded = false;
        if(loaded) return true;
        loaded = true;

        log::debug("Loading song browser lag fix");

        populateDownloadedSongsFast();

        return true;
    }
};