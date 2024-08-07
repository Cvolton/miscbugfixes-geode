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
        CommentCell::onLike(sender);

        if(typeinfo_cast<LikeItemLayer*>(CCScene::get()->getChildren()->lastObject())) {
            this->retain();
            m_fields->m_retained = true;
        }
    }

    void likedItem(LikeItemType type, int id, int special) {
        CommentCell::likedItem(type, id, special);

        if(m_fields->m_retained) this->release();
        m_fields->m_retained = false;
    }
};