#include <Geode/Geode.hpp>
#include "StudioLayer.hpp"
#include "VeyronStudio.hpp"

#include <sstream>
#include <string>

using namespace geode::prelude;

namespace Veyron::UI {

class StudioPopup final : public CCLayerColor {
public:
    static StudioPopup* create() {
        auto* popup = new StudioPopup();
        if (popup && popup->init()) {
            popup->autorelease();
            return popup;
        }
        CC_SAFE_DELETE(popup);
        return nullptr;
    }

    bool init() override {
        if (!CCLayerColor::initWithColor(ccc4(5, 8, 14, 224))) return false;

        const auto win = CCDirector::sharedDirector()->getWinSize();
        this->setContentSize(win);

        const float panelWidth = win.width * 0.84f;
        const float panelHeight = win.height * 0.82f;
        const float panelX = (win.width - panelWidth) * 0.5f;
        const float panelY = (win.height - panelHeight) * 0.5f;

        auto* panel = CCLayerColor::create(ccc4(17, 22, 33, 250));
        if (!panel) return false;
        panel->setContentSize({panelWidth, panelHeight});
        panel->setPosition({panelX, panelY});
        panel->setScale(0.94f);
        this->addChild(panel, 1);

        auto* accent = CCLayerColor::create(ccc4(75, 190, 230, 255));
        accent->setContentSize({panelWidth, 3.0f});
        accent->setPosition({0.0f, panelHeight - 3.0f});
        panel->addChild(accent, 2);

        auto* title = CCLabelBMFont::create("VEYRON STUDIO", "bigFont.fnt");
        title->setScale(0.72f);
        title->setColor(ccc3(225, 242, 255));
        title->setPosition({panelWidth * 0.5f, panelHeight - 39.0f});
        panel->addChild(title, 3);

        auto* subtitle = CCLabelBMFont::create("CREATOR TOOLKIT  //  BUILD 1.1.0  //  WINDOWS", "bigFont.fnt");
        subtitle->setScale(0.34f);
        subtitle->setColor(ccc3(116, 153, 178));
        subtitle->setPosition({panelWidth * 0.5f, panelHeight - 68.0f});
        panel->addChild(subtitle, 3);

        auto* divider = CCLayerColor::create(ccc4(57, 77, 99, 255));
        divider->setContentSize({panelWidth - 56.0f, 1.0f});
        divider->setPosition({28.0f, panelHeight - 89.0f});
        panel->addChild(divider, 2);

        auto* closeLabel = CCLabelBMFont::create("X", "bigFont.fnt");
        closeLabel->setScale(0.60f);
        closeLabel->setColor(ccc3(245, 135, 145));
        auto* closeButton = CCMenuItemLabel::create(closeLabel, this, menu_selector(StudioPopup::onClose));
        closeButton->setPosition({panelWidth - 28.0f, panelHeight - 27.0f});

        auto* section = CCLabelBMFont::create("OVERVIEW", "bigFont.fnt");
        section->setScale(0.52f);
        section->setColor(ccc3(105, 205, 240));
        section->setAnchorPoint({0.0f, 0.5f});
        section->setPosition({32.0f, panelHeight - 203.0f});
        panel->addChild(section, 3);
        m_sectionTitle = section;

        const std::string initialText = homeText();
        auto* content = CCLabelBMFont::create(initialText.c_str(), "bigFont.fnt", panelWidth - 64.0f, kCCTextAlignmentLeft);
        content->setScale(0.43f);
        content->setAnchorPoint({0.0f, 1.0f});
        content->setColor(ccc3(195, 207, 220));
        content->setPosition({32.0f, panelHeight - 226.0f});
        panel->addChild(content, 3);
        m_content = content;

        auto* footerLine = CCLayerColor::create(ccc4(57, 77, 99, 255));
        footerLine->setContentSize({panelWidth - 56.0f, 1.0f});
        footerLine->setPosition({28.0f, 48.0f});
        panel->addChild(footerLine, 2);

        const bool showHints = Mod::get()->getSettingValue<bool>("show-tool-hints");
        const char* footerText = showHints
            ? "LOCAL TOOLING PREVIEW  |  SELECT A TAB TO EXPLORE THE MODULES"
            : "VEYRON STUDIO 1.1.0  |  WINDOWS x64";
        auto* footer = CCLabelBMFont::create(footerText, "bigFont.fnt");
        footer->setScale(0.30f);
        footer->setColor(ccc3(100, 126, 148));
        footer->setPosition({panelWidth * 0.5f, 27.0f});
        panel->addChild(footer, 3);

        auto* navMenu = CCMenu::create();
        navMenu->setPosition({0.0f, 0.0f});
        panel->addChild(navMenu, 4);
        addNavButton(navMenu, "HOME", panelWidth * 0.13f, panelHeight - 119.0f, menu_selector(StudioPopup::onHome));
        addNavButton(navMenu, "ANALYZER", panelWidth * 0.37f, panelHeight - 119.0f, menu_selector(StudioPopup::onAnalyzer));
        addNavButton(navMenu, "FRAME LAB", panelWidth * 0.63f, panelHeight - 119.0f, menu_selector(StudioPopup::onFrames));
        addNavButton(navMenu, "ID MAP", panelWidth * 0.87f, panelHeight - 119.0f, menu_selector(StudioPopup::onIDs));
        addNavButton(navMenu, "SNAPSHOTS", panelWidth * 0.25f, panelHeight - 160.0f, menu_selector(StudioPopup::onSnapshots));
        addNavButton(navMenu, "VISUALIZER", panelWidth * 0.50f, panelHeight - 160.0f, menu_selector(StudioPopup::onVisualizer));
        addNavButton(navMenu, "SETTINGS", panelWidth * 0.75f, panelHeight - 160.0f, menu_selector(StudioPopup::onSettings));
        navMenu->addChild(closeButton);

        if (Mod::get()->getSettingValue<bool>("animate-ui")) {
            panel->runAction(CCSequence::create(
                CCScaleTo::create(0.16f, 1.015f),
                CCScaleTo::create(0.10f, 1.0f),
                nullptr
            ));
            title->runAction(CCRepeatForever::create(CCSequence::create(
                CCTintTo::create(1.25f, 175, 225, 255),
                CCTintTo::create(1.25f, 225, 242, 255),
                nullptr
            )));
        }

        return true;
    }

private:
    CCLabelBMFont* m_sectionTitle = nullptr;
    CCLabelBMFont* m_content = nullptr;

    void addNavButton(CCMenu* menu, const char* text, float x, float y, SEL_MenuHandler handler) {
        auto* label = CCLabelBMFont::create(text, "bigFont.fnt");
        label->setScale(0.40f);
        label->setColor(ccc3(136, 185, 211));
        auto* item = CCMenuItemLabel::create(label, this, handler);
        item->setPosition({x, y});
        menu->addChild(item);
    }

    void setPage(const char* title, const std::string& body) {
        if (m_sectionTitle) m_sectionTitle->setString(title);
        if (m_content) {
            m_content->stopAllActions();
            m_content->setString(body.c_str());
            if (Mod::get()->getSettingValue<bool>("animate-ui")) {
                m_content->setOpacity(0);
                m_content->runAction(CCFadeTo::create(0.18f, 255));
            } else {
                m_content->setOpacity(255);
            }
        }
    }

    std::string homeText() const {
        return
            "WELCOME TO YOUR CREATOR WORKSPACE\n\n"
            "Veyron Studio is a modular toolkit for level creators, decorators and verifiers.\n\n"
            "MODULES ONLINE\n"
            "- Level diagnostics and data validation\n"
            "- Frame-gap statistics and timing summaries\n"
            "- Group and color ID usage maps\n"
            "- In-memory snapshots and change comparisons\n"
            "- Horizontal structure summaries\n\n"
            "IMPORTANT: this build does not read the currently open GD editor level yet. The Analyzer tab uses a clearly labelled bundled demo dataset.";
    }

    void onHome(CCObject*) { setPage("OVERVIEW", homeText()); }

    void onAnalyzer(CCObject*) {
        const auto demo = Veyron::StudioManager::get().makeDemoLevel();
        const auto diagnostics = Veyron::LevelAnalyzer().analyze(demo, static_cast<int>(Mod::get()->getSettingValue<int64_t>("diagnostic-sensitivity")));
        std::ostringstream text;
        text << "DIAGNOSTIC ENGINE // BUNDLED DEMO DATA\n\n"
             << "Dataset: " << demo.name << '\n'
             << "Object records: " << demo.objects.size() << '\n'
             << "Trigger records: " << Veyron::LevelDataStore::countTriggers(demo) << "\n\n"
             << Veyron::Diagnostics::format(diagnostics)
             << "\nThis is a test fixture, not a scan of your current level.";
        setPage("ANALYZER", text.str());
    }

    void onFrames(CCObject*) {
        const std::vector<int> demoFrames{10, 11, 19, 32, 34, 35, 58, 83, 84, 85};
        const auto stats = Veyron::FrameAnalyzer::analyzeInputFrames(demoFrames);
        std::ostringstream text;
        text << "FRAME LAB // INPUT GAP PREVIEW\n\n"
             << "Sample input events: " << stats.inputCount << '\n'
             << "Smallest gap: " << stats.smallestGap << " frame(s)\n"
             << "Largest gap: " << stats.largestGap << " frame(s)\n"
             << "Average gap: " << stats.averageGap << " frame(s)\n"
             << "One-frame gaps: " << stats.oneFrameGaps << '\n'
             << "Two-frame gaps: " << stats.twoFrameGaps << "\n\n"
             << "The analysis engine accepts frame indices. Live input capture and macro import are not wired into this release yet.";
        setPage("FRAME LAB", text.str());
    }

    void onIDs(CCObject*) {
        const auto demo = Veyron::StudioManager::get().makeDemoLevel();
        const auto groups = Veyron::GroupManager::countUsage(demo);
        const auto colors = Veyron::ColorManager::countUsage(demo);
        std::ostringstream text;
        text << "ID MAP // BUNDLED DEMO DATA\n\nGROUP USAGE\n";
        for (const auto& [id, count] : groups) text << "Group " << id << "  ->  " << count << " record(s)\n";
        text << "\nCOLOR USAGE\n";
        for (const auto& [id, count] : colors) text << "Color " << id << "  ->  " << count << " record(s)\n";
        text << "\nUnused-ID scanning is available through the core manager when a level dataset is supplied.";
        setPage("ID MAP", text.str());
    }

    void onSnapshots(CCObject*) {
        const auto original = Veyron::StudioManager::get().makeDemoLevel();
        Veyron::SnapshotManager snapshots;
        snapshots.takeSnapshot(original);
        auto edited = original;
        if (!edited.objects.empty()) edited.objects.pop_back();
        edited.objects.push_back({211, 8, 4, 1600.0, 48.0, false});
        const auto diff = snapshots.compareWithLatest(edited);
        std::ostringstream text;
        text << "SNAPSHOT MANAGER // MEMORY DEMO\n\n"
             << "Snapshots in this session: " << snapshots.snapshotCount() << '\n'
             << "Added records: " << diff.added << '\n'
             << "Removed records: " << diff.removed << '\n'
             << "Unchanged estimate: " << diff.unchanged << "\n\n"
             << "This build compares snapshots in memory. Persistent save slots and rollback into GD are not enabled.";
        setPage("SNAPSHOTS", text.str());
    }

    void onVisualizer(CCObject*) {
        const auto demo = Veyron::StudioManager::get().makeDemoLevel();
        setPage("VISUALIZER", Veyron::StructureVisualizer::buildSummary(demo));
    }

    void onSettings(CCObject*) {
        const bool animations = Mod::get()->getSettingValue<bool>("animate-ui");
        const bool hints = Mod::get()->getSettingValue<bool>("show-tool-hints");
        const auto sensitivity = Mod::get()->getSettingValue<int64_t>("diagnostic-sensitivity");
        std::ostringstream text;
        text << "PREFERENCES // GEODE MOD SETTINGS\n\n"
             << "Interface animations: " << (animations ? "ON" : "OFF") << '\n'
             << "Tool hints: " << (hints ? "ON" : "OFF") << '\n'
             << "Diagnostic sensitivity: " << sensitivity << "/5\n\n"
             << "Change these values from Geode's mod settings page.\n"
             << "Veyron Studio targets Geometry Dash 2.2091 on Windows x64.";
        setPage("SETTINGS", text.str());
    }

    void onClose(CCObject*) {
        this->removeFromParentAndCleanup(true);
    }
};


cocos2d::CCLayer* createStudioPopup() {
    return StudioPopup::create();
}

} // namespace Veyron::UI
