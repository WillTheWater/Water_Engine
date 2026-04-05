// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/UIStyle.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Utility/Log.h"

#include <cstring>

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
    
    static vector<uint8> FontDataBuffer;
    
    void UIStyle::Initialize()
    {
        if (bInitialized) return;
        
        bool bLoaded = false;
        
        #ifdef USE_RAW_ASSETS
            // Debug: Load from disk directly
            string ResolvedFontPath = string(ASSET_ROOT_PATH) + FontPath;
            std::ifstream file(ResolvedFontPath, std::ios::binary | std::ios::ate);
            if (file)
            {
                auto size = file.tellg();
                file.seekg(0, std::ios::beg);
                FontDataBuffer.resize(static_cast<ulong>(size));
                file.read(reinterpret_cast<char*>(FontDataBuffer.data()), size);
                file.close();
                bLoaded = true;
            }
            else
            {
                WARNING("UIStyle: Failed to load font file: {}", ResolvedFontPath);
            }
        #else
            // Release: Load from packed assets via ResourceSubsystem
            string FontData = LoadAsset().LoadFileData(FontPath);
            if (!FontData.empty())
            {
                FontDataBuffer.resize(FontData.size());
                memcpy(FontDataBuffer.data(), FontData.data(), FontData.size());
                bLoaded = true;
            }
            else
            {
                WARNING("UIStyle: Failed to load font from pak: {}", FontPath);
            }
        #endif
        
        if (bLoaded && !FontDataBuffer.empty())
        {
            try {
                tgui::Font font(FontDataBuffer.data(), FontDataBuffer.size());
                if (font)
                {
                    tgui::Font::setGlobalFont(font);
                }
            }
            catch (...) {
                WARNING("UIStyle: Failed to set TGUI global font from memory");
            }
        }
        
        GameFont = LoadAsset().LoadFont(FontPath);
        
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
    

    shared<tgui::Button> UIStyle::CreateButton(const string& Text)
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
    
    shared<tgui::CheckBox> UIStyle::CreateCheckbox(const string& Text)
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
    
    shared<tgui::Slider> UIStyle::CreateSlider()
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
    
    shared<tgui::Panel> UIStyle::CreatePanel(tgui::Layout2d Size)
    {
        Initialize();
        
        auto Panel = tgui::Panel::create(Size);
        
        auto Renderer = Panel->getRenderer();
        Renderer->setBackgroundColor(Colors.PanelBackground);
        Renderer->setBorderColor(Colors.BorderColor);
        Renderer->setBorders(tgui::Outline(Colors.BorderWidth));
        
        return Panel;
    }
    
    shared<tgui::Label> UIStyle::CreateLabel(const string& Text, UILabelStyle Style)
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
    
    shared<tgui::VerticalLayout> UIStyle::CreateVerticalLayout()
    {
        auto Layout = tgui::VerticalLayout::create();
        Layout->getRenderer()->setSpaceBetweenWidgets(10);
        Layout->getRenderer()->setPadding(10);
        return Layout;
    }
    
    shared<tgui::HorizontalLayout> UIStyle::CreateHorizontalLayout()
    {
        auto Layout = tgui::HorizontalLayout::create();
        Layout->getRenderer()->setSpaceBetweenWidgets(10);
        return Layout;
    }
}
