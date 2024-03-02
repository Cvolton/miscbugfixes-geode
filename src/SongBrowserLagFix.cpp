#include <Geode/Geode.hpp>
#include <Geode/modify/MusicDownloadManager.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/CustomSongWidget.hpp>

using namespace geode::prelude;

static inline std::unordered_map<int, bool> s_downloadedSongs;

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

    void limitDownloadedSongs() {
        s_downloadedSongs.clear();

        MusicDownloadManager::limitDownloadedSongs();
    }

    void downloadSongFinished(int songID) {
        if(s_downloadedSongs.contains(songID)) s_downloadedSongs[songID] = true;

        MusicDownloadManager::downloadSongFinished(songID);
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
        if(s_downloadedSongs.contains(m_songInfoObject->m_songID)) s_downloadedSongs.erase(m_songInfoObject->m_songID);

        CustomSongWidget::updateSongInfo();
    }
};