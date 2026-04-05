// =============================================================================
// Water Engine v2.0.0 - Demo Game
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
        // Initialize - must be called before using any style functions
        // Loads the custom font from Assets
        static void Initialize();
        static bool IsInitialized();
        
        // Get the color theme (can be modified at runtime)
        static UIColorTheme& GetColors();
        
        // Get font sizes
        static UIFontSizes& GetFontSizes();
        
        // Get size constants
        static UISizes& GetSizes();
        
        // Get the loaded custom font
        static shared<font> GetFont();
        
        //=========================================================================
        // WIDGET FACTORY METHODS - All styled consistently
        //=========================================================================
        
        static shared<tgui::Button> CreateButton(const string& Text);
        
        // Creates a standard checkbox
        static shared<tgui::CheckBox> CreateCheckbox(const string& Text = "");
        
        // Creates a standard slider (0-100 range)
        static shared<tgui::Slider> CreateSlider();
        
        // Creates a standard panel with border
        static shared<tgui::Panel> CreatePanel(tgui::Layout2d Size);
        
        // Creates a label with specified style tier
        static shared<tgui::Label> CreateLabel(const string& Text, UILabelStyle Style = UILabelStyle::Body);
        
        // Creates a vertical layout with standard spacing
        static shared<tgui::VerticalLayout> CreateVerticalLayout();
        
        // Creates a horizontal layout
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