// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/UIStyle.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Log.h"

#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>
#include <TGUI/Widgets/Slider.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/VerticalLayout.hpp>
#include <TGUI/Widgets/HorizontalLayout.hpp>

namespace we
{
    const char* const UIStyle::FontPath = "Assets/Font/Default/defaultFont.otf";
    
    bool UIStyle::bInitialized = false;
    UIColorTheme UIStyle::Colors;
    UIFontSizes UIStyle::FontSizes;
    UISizes UIStyle::Sizes;
    shared<font> UIStyle::GameFont;
    
    void UIStyle::Initialize()
    {
        if (bInitialized) return;
        
        // Resolve the font path (handles packed vs raw assets)
        std::string ResolvedFontPath;
        #ifdef USE_RAW_ASSETS
            ResolvedFontPath = std::string(ASSET_ROOT_PATH) + FontPath;
        #else
            ResolvedFontPath = FontPath;
        #endif
        
        // Load custom font via ResourceSubsystem to keep it cached
        GameFont = LoadAsset().LoadFont(FontPath);
        if (!GameFont)
        {
            WARNING("UIStyle: Failed to load custom font from {}", FontPath);
        }
        else
        {
            LOG("UIStyle: Loaded custom font from {}", FontPath);
            
            // Set TGUI global font (font object is temporary, TGUI copies it)
            try {
                tgui::Font TGUIFont(ResolvedFontPath);
                if (TGUIFont)
                {
                    tgui::Font::setGlobalFont(TGUIFont);
                    LOG("UIStyle: Set TGUI global font");
                }
            }
            catch (...) {
                WARNING("UIStyle: Failed to set TGUI global font");
            }
        }
        
        bInitialized = true;
    }
    
    bool UIStyle::IsInitialized()
    {
        return bInitialized;
    }
    
    UIColorTheme& UIStyle::GetColors()
    {
        return Colors;
    }
    
    UIFontSizes& UIStyle::GetFontSizes()
    {
        return FontSizes;
    }
    
    UISizes& UIStyle::GetSizes()
    {
        return Sizes;
    }
    
    shared<font> UIStyle::GetFont()
    {
        Initialize();
        return GameFont;
    }
    

    std::shared_ptr<tgui::Button> UIStyle::CreateButton(const std::string& Text)
    {
        Initialize();
        
        auto Button = tgui::Button::create(Text);
        Button->setTextSize(Sizes.ButtonTextSize);
        Button->setFocusable(false);
        
        auto Renderer = Button->getRenderer();
        Renderer->setBackgroundColor(Colors.PrimaryNormal);
        Renderer->setBackgroundColorHover(Colors.PrimaryHover);
        Renderer->setBackgroundColorDown(Colors.PrimaryActive);
        Renderer->setTextColor(Colors.TextPrimary);
        Renderer->setTextColorHover(Colors.TextPrimary);
        Renderer->setTextColorDown(Colors.TextPrimary);
        Renderer->setBorderColor(Colors.BorderColor);
        Renderer->setBorders(tgui::Outline(Colors.BorderWidth));
        
        return Button;
    }
    
    std::shared_ptr<tgui::CheckBox> UIStyle::CreateCheckbox(const std::string& Text)
    {
        Initialize();
        
        auto Checkbox = tgui::CheckBox::create(Text);
        Checkbox->setSize(Sizes.CheckboxSize);
        Checkbox->setTextSize(Sizes.CheckboxTextSize);
        Checkbox->setFocusable(false);
        
        auto Renderer = Checkbox->getRenderer();
        Renderer->setBackgroundColor(Colors.PrimaryNormal);
        Renderer->setBackgroundColorHover(Colors.PrimaryHover);
        Renderer->setBackgroundColorChecked(Colors.PrimaryActive);
        Renderer->setBackgroundColorCheckedHover(Colors.SecondaryHover);
        Renderer->setBorderColor(Colors.BorderColor);
        Renderer->setBorders(tgui::Outline(Colors.BorderWidth));
        Renderer->setTextColor(Colors.TextPrimary);
        
        return Checkbox;
    }
    
    std::shared_ptr<tgui::Slider> UIStyle::CreateSlider()
    {
        Initialize();
        
        auto Slider = tgui::Slider::create();
        Slider->setMinimum(0);
        Slider->setMaximum(100);
        Slider->setValue(100);
        Slider->setStep(1);
        Slider->setFocusable(false);
        
        auto Renderer = Slider->getRenderer();
        Renderer->setTrackColor(Colors.PrimaryNormal);
        Renderer->setTrackColorHover(Colors.PrimaryHover);
        Renderer->setThumbColor(Colors.PrimaryActive);
        Renderer->setThumbColorHover(Colors.SecondaryHover);
        Renderer->setBorderColor(Colors.BorderColor);
        Renderer->setBorders(tgui::Outline(Colors.BorderWidth));
        
        return Slider;
    }
    
    std::shared_ptr<tgui::Panel> UIStyle::CreatePanel(tgui::Layout2d Size)
    {
        Initialize();
        
        auto Panel = tgui::Panel::create(Size);
        
        auto Renderer = Panel->getRenderer();
        Renderer->setBackgroundColor(Colors.PanelBackground);
        Renderer->setBorderColor(Colors.BorderColor);
        Renderer->setBorders(tgui::Outline(Colors.BorderWidth));
        
        return Panel;
    }
    
    std::shared_ptr<tgui::Label> UIStyle::CreateLabel(const std::string& Text, UILabelStyle Style)
    {
        Initialize();
        
        auto Label = tgui::Label::create(Text);
        
        unsigned int TextSize = FontSizes.Body;
        tgui::Color Color = Colors.TextPrimary;
        
        switch (Style)
        {
            case UILabelStyle::Title:
                TextSize = FontSizes.Title;
                break;
            case UILabelStyle::Section:
                TextSize = FontSizes.Section;
                Color = Colors.TextSecondary;
                break;
            case UILabelStyle::Subsection:
                TextSize = FontSizes.Subsection;
                break;
            case UILabelStyle::Body:
                TextSize = FontSizes.Body;
                break;
            case UILabelStyle::Small:
                TextSize = FontSizes.Small;
                break;
            case UILabelStyle::Tiny:
                TextSize = FontSizes.Tiny;
                break;
        }
        
        Label->setTextSize(TextSize);
        Label->getRenderer()->setTextColor(Color);
        
        return Label;
    }
    
    std::shared_ptr<tgui::VerticalLayout> UIStyle::CreateVerticalLayout()
    {
        auto Layout = tgui::VerticalLayout::create();
        Layout->getRenderer()->setSpaceBetweenWidgets(10);
        Layout->getRenderer()->setPadding(10);
        return Layout;
    }
    
    std::shared_ptr<tgui::HorizontalLayout> UIStyle::CreateHorizontalLayout()
    {
        auto Layout = tgui::HorizontalLayout::create();
        Layout->getRenderer()->setSpaceBetweenWidgets(10);
        return Layout;
    }
}
