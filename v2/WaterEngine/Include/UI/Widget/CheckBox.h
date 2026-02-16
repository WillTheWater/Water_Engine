//// =============================================================================
//// Water Engine v2.0.0
//// Copyright(C) 2026 Will The Water
//// =============================================================================
//
//#pragma once
//
//#include "Core/CoreMinimal.h"
//#include "UI/Widget/Widget.h"
//#include "Framework/EngineSubsystem.h"
//#include <SFML/Graphics/RectangleShape.hpp>
//#include <SFML/Graphics/Text.hpp>
//
//namespace we
//{
//    class Checkbox : public Widget
//    {
//    public:
//        Checkbox(
//            EngineSubsystem& Subsystem,
//            const string& LabelText,
//            bool bInitialState = false,
//            float BoxSize = 24.f
//        );
//
//        void Update(float DeltaTime) override;
//        void Render(GameWindow& Window) override;
//
//        void OnMouseEnter() override;
//        void OnMouseLeave() override;
//        bool OnMouseButtonPressed() override;
//        void OnMouseButtonReleased() override;
//
//        void SetChecked(bool bChecked);
//        bool IsChecked() const { return bChecked; }
//
//        void Toggle();
//
//        void SetLabel(const string& NewLabel);
//        const string& GetLabel() const { return LabelText; }
//
//        void SetBoxSize(float Size);
//        float GetBoxSize() const { return BoxSize; }
//
//        void SetLabelOffset(float Offset);
//        float GetLabelOffset() const { return LabelOffset; }
//
//        void SetUncheckedColor(color Color);
//        void SetCheckedColor(color Color);
//        void SetHoverColor(color Color);
//
//        Delegate<bool> OnToggled;
//
//    private:
//        void UpdateVisualState();
//        void UpdateSize();
//
//        string LabelText;
//        bool bChecked{ false };
//        bool bHovered{ false };
//        bool bPressed{ false };
//
//        float BoxSize;
//        float LabelOffset{ 10.f };
//
//        color UncheckedColor{ 80, 80, 80 };
//        color CheckedColor{ 100, 200, 100 };
//        color HoverColor{ 220, 220, 220 };
//
//        shared<font> TextFont;
//        optional<sf::Text> LabelDrawable;
//        optional<rectangle> BoxDrawable;
//    };
//}