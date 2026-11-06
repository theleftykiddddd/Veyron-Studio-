#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>

#include "StudioLayer.hpp"

using namespace geode::prelude;

class $modify(VeyronMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto* rightMenu = this->getChildByID("right-menu");
        if (!rightMenu || rightMenu->getChildByID("veyron-studio-button")) return true;

        auto* label = CCLabelBMFont::create("V", "bigFont.fnt");
        if (!label) return true;
        label->setColor(ccc3(105, 205, 240));
        label->setScale(0.85f);

        auto* button = CCMenuItemLabel::create(label, this, menu_selector(VeyronMenuLayer::onVeyronStudio));
        if (!button) return true;
        button->setID("veyron-studio-button");
        rightMenu->addChild(button);
        rightMenu->updateLayout();

        if (Mod::get()->getSettingValue<bool>("animate-ui")) {
            button->runAction(CCRepeatForever::create(CCSequence::create(
                CCScaleTo::create(0.85f, 1.08f),
                CCScaleTo::create(0.85f, 0.94f),
                nullptr
            )));
        }
        return true;
    }

    void onVeyronStudio(CCObject*) {
        auto* scene = CCDirector::sharedDirector()->getRunningScene();
        if (!scene || scene->getChildByID("veyron-studio-popup")) return;

        auto* popup = Veyron::UI::createStudioPopup();
        if (!popup) {
            FLAlertLayer::create("Veyron Studio", "The dashboard could not be initialized.", "OK")->show();
            return;
        }
        popup->setID("veyron-studio-popup");
        scene->addChild(popup, 10000);
    }
};
