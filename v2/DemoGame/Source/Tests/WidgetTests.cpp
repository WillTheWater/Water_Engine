// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Tests/WidgetTests.h"
#include "Subsystem/GuiSubsystem.h"
#include "Utility/Log.h"

#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/Label.hpp>

namespace we
{
    void WidgetTestSetup::CreateScreenUI()
    {
        LOG("WidgetTestSetup: Creating ScreenUI widgets");
        
        auto Button = tgui::Button::create("Test Button");
        Button->setPosition({100, 100});
        Button->setSize({200, 50});
        
        // Set colors to visualize hover (blue normal, green hover)
        auto Renderer = Button->getRenderer();
        Renderer->setBackgroundColor(tgui::Color::White);
        Renderer->setBackgroundColorHover(tgui::Color::Blue);
        
        Button->onPress([](){
            LOG("Test button clicked!");
        });
        MakeGUI().GetScreenUI().add(Button);
        
        auto Label = tgui::Label::create("Hello TGUI!");
        Label->setPosition({100, 50});
        Label->setTextSize(24);
        MakeGUI().GetScreenUI().add(Label);
    }

    void WidgetTestSetup::CreateWorldUI()
    {
        LOG("WidgetTestSetup: Creating WorldUI widgets");
        
        auto Button = tgui::Button::create("World Button");
        Button->setPosition({500, 300});
        Button->setSize({150, 40});
        MakeGUI().GetWorldUI().add(Button);
        
        auto Label = tgui::Label::create("World Label");
        Label->setPosition({500, 270});
        MakeGUI().GetWorldUI().add(Label);
    }

    void WidgetTestSetup::RemoveAll()
    {
        MakeGUI().GetScreenUI().removeAllWidgets();
        MakeGUI().GetWorldUI().removeAllWidgets();
        LOG("WidgetTestSetup: Removed all widgets");
    }
}
