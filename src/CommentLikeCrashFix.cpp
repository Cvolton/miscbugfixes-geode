#include <Geode/Geode.hpp>
#include <Geode/modify/CommentCell.hpp>

using namespace geode::prelude;

class $modify(CommentCell) {
    struct Fields {
        bool m_retained = false;
    };

    static void onModify(auto& self) {
        auto res = self.setHookPriority("CommentCell::onLike", 0x10000);
    }

    void onConfirmDelete(CCObject* sender) {
        this->retain();
        m_fields->m_retained = true;
        CommentCell::onConfirmDelete(sender);
    }

    void FLAlert_Clicked(FLAlertLayer* layer, bool btn) {
        CommentCell::FLAlert_Clicked(layer, btn);

        if(m_fields->m_retained) this->release();
        m_fields->m_retained = false;
    }

    void onLike(CCObject* sender) {
        static_assert(&CommentCell::onLike);

        if(!this->m_comment) return;

        LikeItemType type = LikeItemType::Comment;
        int special = this->m_comment->m_levelID;

        if (this->m_accountComment) {
            special = this->m_comment->m_accountID;
            type = LikeItemType::AccountComment;
        }

        bool liked = GameLevelManager::sharedState()->hasLikedItemFullCheck(type, this->m_comment->m_commentID, special);
        if(liked) return;

        auto GM = GameManager::sharedState();
        if (this->m_comment->m_userID == GM->m_playerUserID) return;
        
        auto AM = GJAccountManager::sharedState();
        if (this->m_comment->m_accountID && this->m_comment->m_accountID == AM->m_accountID) return;

        auto layer = LikeItemLayer::create(type, this->m_comment->m_commentID, special);
        layer->m_likeDelegate = this;
        layer->show();
        this->retain();
        m_fields->m_retained = true;
    }

    void likedItem(LikeItemType type, int id, int special) {
        CommentCell::likedItem(type, id, special);

        if(m_fields->m_retained) this->release();
        m_fields->m_retained = false;
    }
};