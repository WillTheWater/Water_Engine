// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Levels/MainMenu.h"
#include "Framework/EngineSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Levels/LevelOne.h"

namespace we
{
    MainMenu::MainMenu(EngineSubsystem& Subsystem)
        : World(Subsystem)
    {
        BgTexture = Asset().LoadTexture(EC.DefaultBackground);
        Background.emplace(*BgTexture);
        CreateUI();
    }

    MainMenu::~MainMenu()
    {
        if (UIGroup)
        {
            Subsystem.GUI->RemoveWidget(UIGroup);
        }
    }

    void MainMenu::Render()
    {
        if (Background) { Subsystem.Render->Draw(*Background); }

        RenderActors();
    }

    void MainMenu::CreateUI()
    {
        // Create a group to hold all menu widgets
        UIGroup = tgui::Group::create();

        // Colors
        tgui::Color BackgroundColor{38,92,112};
        tgui::Color HoverColor{ 84,137,155 };
        tgui::Color PressedColor{23,74,106};
        tgui::Color BorderColor{ 84,137,155 };
        tgui::Color BorderFocusColor{252,252,252};
        tgui::Color TextColor{252,252,252};
        int FontSize{ 52 };
        int BoarderSize{ 4 };

        // Create a vertical layout that automatically arranges children vertically
        // Layouts handle positioning and sizing of child widgets
        auto MenuLayout = tgui::VerticalLayout::create();
        MenuLayout->setOrigin(0.5f, 0.5f); // Center Origin

        // Position the layout in the center of the screen
        // x=50% from left, y=80% from top
        MenuLayout->setPosition("50%", "80%");

        // Size the layout to take up 20% of screen width, 20% of height
        MenuLayout->setSize("20%", "20%");

        // Space between widgets in the layout (pixels)
        MenuLayout->getRenderer()->setSpaceBetweenWidgets(20);

        // Create Play button
        PlayButton = tgui::Button::create("Play");
        PlayButton->setTextSize(FontSize); // Larger text

        // Style the button colors (idle, hover, pressed states)
        PlayButton->getRenderer()->setBackgroundColor(BackgroundColor);
        PlayButton->getRenderer()->setBackgroundColorHover(HoverColor);
        PlayButton->getRenderer()->setBackgroundColorDown(PressedColor);

        PlayButton->getRenderer()->setTextColor(TextColor);
        PlayButton->getRenderer()->setTextColorHover(TextColor);
        PlayButton->getRenderer()->setTextColorDown(TextColor);

        PlayButton->getRenderer()->setBorders(BoarderSize);
        PlayButton->getRenderer()->setBorderColor(BorderColor);
        PlayButton->getRenderer()->setBorderColorHover(BorderColor);
        PlayButton->getRenderer()->setBorderColorDown(BorderColor);
        PlayButton->getRenderer()->setBorderColorFocused(BorderFocusColor);

        // Bind click handler
        PlayButton->onPress([this]() { HandlePlay(); });

        // Create Quit button with same styling
        QuitButton = tgui::Button::create("Quit");
        QuitButton->setTextSize(FontSize);
        QuitButton->getRenderer()->setBackgroundColor(BackgroundColor);
        QuitButton->getRenderer()->setBackgroundColorHover(HoverColor);
        QuitButton->getRenderer()->setBackgroundColorDown(PressedColor);

        QuitButton->getRenderer()->setTextColor(TextColor);
        QuitButton->getRenderer()->setTextColorHover(TextColor);
        QuitButton->getRenderer()->setTextColorDown(TextColor);

        QuitButton->getRenderer()->setBorders(BoarderSize);
        QuitButton->getRenderer()->setBorderColor(BorderColor);
        QuitButton->getRenderer()->setBorderColorHover(BorderColor);
        QuitButton->getRenderer()->setBorderColorDown(BorderColor);
        QuitButton->getRenderer()->setBorderColorFocused(BorderFocusColor);
        QuitButton->onPress([this]() { HandleQuit(); });

        // Add buttons to layout - they fill the layout's width automatically
        // Height is distributed evenly among children
        MenuLayout->add(PlayButton);
        MenuLayout->add(QuitButton);

        // Add the layout to our group
        UIGroup->add(MenuLayout);

        // Register with GUI subsystem for rendering/input
        Subsystem.GUI->AddWidget(UIGroup);
    }

    void MainMenu::HandlePlay()
    {
        LOG("Play clicked!");
        OnPlayPressed.Broadcast();
    }

    void MainMenu::HandleQuit()
    {
        LOG("Quit clicked!");
        OnQuitPressed.Broadcast();
    }
}