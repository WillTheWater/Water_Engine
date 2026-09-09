// =============================================================================
// Water Engine v2.1.2 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include <TGUI/Core.hpp>

// TGUI Widget forward declarations
namespace tgui
{
    class Button;
    class CheckBox;
    class Slider;
    class Panel;
    class Label;
    class VerticalLayout;
    class HorizontalLayout;
}

namespace we
{
    class ResourceSubsystem;
    
    struct UIColorTheme
    {
        // Primary colors (Button, Checkbox, Slider track)
        tgui::Color PrimaryNormal   = {0, 16, 31};       // Deep blue-black
        tgui::Color PrimaryHover    = {47, 121, 142};    // Teal highlight
        tgui::Color PrimaryActive   = {133, 120, 81};    // Gold/tan when pressed/checked
        
        // Secondary colors (Slider thumb accents)
        tgui::Color SecondaryHover  = {200, 180, 120};   // Light gold for thumb hover
        
        // Background colors
        tgui::Color PanelBackground = {0, 0, 0, 180};    // Semi-transparent black
        tgui::Color Transparent     = {0, 0, 0, 0};
        
        // Text colors
        tgui::Color TextPrimary     = tgui::Color::White;
        tgui::Color TextSecondary   = {200, 200, 200};   // Light gray for subtitles
        tgui::Color TextMuted       = {150, 150, 150};   // For less important text
        
        // Border
        tgui::Color BorderColor     = tgui::Color::White;
        float       BorderWidth     = 2.0f;
    };
    
    struct UIFontSizes
    {
        unsigned int Title       = 52;  // "HOW TO PLAY", Main title
        unsigned int Section     = 42;  // "AUDIO", "VIDEO" section headers
        unsigned int Subsection  = 34;  // Button text, subsection headers
        unsigned int Body        = 24;  // Descriptions, labels
        unsigned int Small       = 16;  // Dialog text, smaller labels
        unsigned int Tiny        = 14;  // Continue arrows, hints
    };
    
    // Size constants
    struct UISizes
    {
        // Button
        unsigned int   ButtonTextSize  = 24;
        
        // Checkbox
        tgui::Layout2d CheckboxSize    = {32, 32};  // Increased from 25x25
        unsigned int   CheckboxTextSize= 18;
        
        // Slider
        unsigned int   SliderTextSize  = 16;
        
        // Dialog/Panel
        float          DialogWidth     = 400.0f;
        float          DialogHeight    = 120.0f;
        float          TutorialWidth   = 0.9f;   // 90% of screen
        float          TutorialHeight  = 0.9f;
        float          CreditsDialogW  = 500.0f;
        float          CreditsDialogH  = 200.0f;
    };
    
    // Label style tiers
    enum class UILabelStyle 
    { 
        Title,      
        Section,    
        Subsection, 
        Body,       
        Small,      
        Tiny        
    };
    
    class UIStyle
    {
    public:
        // Loads the font and sets it as TGUI's global font. Idempotent, and the
        // members that need the font call it themselves.
        static void Initialize();
        static bool IsInitialized();

        // Returned by reference, safe to modify at runtime
        static UIColorTheme& GetColors();
        static UIFontSizes& GetFontSizes();
        static UISizes& GetSizes();

        static shared<font> GetFont();

        // =========================================================================
        // Widget Factories
        // =========================================================================

        static shared<tgui::Button> CreateButton(const string& Text);
        static shared<tgui::CheckBox> CreateCheckbox(const string& Text = "");

        // Range 0-100
        static shared<tgui::Slider> CreateSlider();

        static shared<tgui::Panel> CreatePanel(tgui::Layout2d Size);
        static shared<tgui::Label> CreateLabel(const string& Text, UILabelStyle Style = UILabelStyle::Body);
        static shared<tgui::VerticalLayout> CreateVerticalLayout();
        static shared<tgui::HorizontalLayout> CreateHorizontalLayout();
        
    private:
        static bool bInitialized;
        static UIColorTheme Colors;
        static UIFontSizes FontSizes;
        static UISizes Sizes;
        static shared<font> GameFont;        
        static const char* const FontPath;
    };
}