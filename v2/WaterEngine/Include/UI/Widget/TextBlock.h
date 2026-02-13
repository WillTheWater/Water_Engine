// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Utility/CoreMinimal.h"
#include "UI/Widget/Widget.h"

namespace we
{
    class TextBlock : public Widget
    {
    public:
        TextBlock(EngineSubsystem& InSubsystem, const string& InText, float InWidth, float LineSpace = 2.f);

        void SetText(const string& InText);
        const string& GetText() const;

        void SetFont(shared<font> InFont);
        void SetCharacterSize(uint Size);
        void SetColor(const color& InColor);

        void Render(GameWindow& Window) override;

    private:
        void UpdateWrappedText();

        string RawText;
        shared<font> FontResource;
        sf::Text TextComponent;
        uint CharSize = 30;
        float LineSpacing;
        bool bNeedsWrapUpdate = true;
    
    };
}