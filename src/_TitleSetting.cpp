#include <Geode/Geode.hpp>
#include <Geode/loader/SettingNode.hpp>

using namespace geode::prelude;

class TitleSettingNode : public SettingNode {
protected:
    bool init(SettingValue* value, float width) {
        if (!SettingNode::init(value)) return false;
        this->setContentSize({ width, 40.f });

        auto title = Mod::get()->getSettingDefinition(value->getKey())->get<CustomSetting>()->json->get<std::string>("name");

        auto label = CCLabelBMFont::create(title.c_str(), "goldFont.fnt");
        label->setPosition({ this->getContentWidth() / 2, this->getContentHeight() / 2 });
        label->setScale(.6f);

        this->addChild(label);

        return true;
    }

public:
    void commit() override { this->dispatchCommitted(); }
    bool hasUncommittedChanges() override { return false; }
    bool hasNonDefaultValue() override { return false; }

    void resetToDefault() override {}

    static TitleSettingNode* create(SettingValue* value, float width) {
        auto ret = new TitleSettingNode();
        if (ret->init(value, width)) {
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
    }
};

class TitleSettingValue : public SettingValue {
public:
    TitleSettingValue(std::string const& key, std::string const& mod, bool value)
      : SettingValue(key, mod) {}

    bool load(matjson::Value const& json) override { return true; }
    bool save(matjson::Value& json) const override { return true; }
    SettingNode* createNode(float width) override {
        return TitleSettingNode::create(this, width);
    }
};

$on_mod(Loaded) {
    Mod::get()->addCustomSetting<TitleSettingValue>("practice-mode-title", false);
    Mod::get()->addCustomSetting<TitleSettingValue>("other-title", false);
}