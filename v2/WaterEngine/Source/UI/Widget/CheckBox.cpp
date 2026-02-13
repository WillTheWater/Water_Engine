// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/Checkbox.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Framework/GameWindow.h"
#include "Utility/Log.h"

namespace we
{
    Checkbox::Checkbox(
        EngineSubsystem& Subsystem,
        const string& InLabel,
        bool bInitialChecked,
        float InBoxSize
    )
        : Widget{ Subsystem }
        , LabelText{ InLabel }
        , bChecked{ bInitialChecked }
        , BoxSize{ InBoxSize }
    {
        // Load font
        TextFont = Asset().LoadFont(EC.DefaultFont);

        // Create checkbox box
        BoxDrawable.emplace();
        BoxDrawable->setSize({ BoxSize, BoxSize });
        BoxDrawable->setOutlineColor(color::Black);
        BoxDrawable->setOutlineThickness(2.f);

        // Create label text
        if (TextFont)
        {
            LabelDrawable.emplace(*TextFont, LabelText, 28);
            LabelDrawable->setFillColor(color::White);
            LabelDrawable->setOutlineColor(color::Black);
            LabelDrawable->setOutlineThickness(1);
        }

        UpdateVisualState();
        UpdateSize();
    }

    void Checkbox::Update(float DeltaTime)
    {
        // Checkboxes don't need per-frame updates
    }

    void Checkbox::Render(GameWindow& Window)
    {
        if (!IsVisible())
            return;

        vec2f Pos = GetWorldPosition();

        if (BoxDrawable)
        {
            BoxDrawable->setPosition(Pos);
            Window.draw(*BoxDrawable);
        }

        // Draw label
        if (LabelDrawable)
        {
            LabelDrawable->setPosition(Pos + vec2f{ BoxSize + LabelOffset, 0.f });
            Window.draw(*LabelDrawable);
        }

        Widget::Render(Window);
    }

    void Checkbox::OnMouseEnter()
    {
        bHovered = true;
        UpdateVisualState();
    }

    void Checkbox::OnMouseLeave()
    {
        bHovered = false;
        bPressed = false;
        UpdateVisualState();
    }

    bool Checkbox::OnMouseButtonPressed()
    {
        bPressed = true;
        UpdateVisualState();
        return true;
    }

    void Checkbox::OnMouseButtonReleased()
    {
        if (bPressed && bHovered)
        {
            Toggle();
        }
        bPressed = false;
        UpdateVisualState();
    }

    void Checkbox::SetChecked(bool bNewChecked)
    {
        if (bChecked != bNewChecked)
        {
            bChecked = bNewChecked;
            OnToggled.Broadcast(bChecked);
        }
    }

    void Checkbox::Toggle()
    {
        SetChecked(!bChecked);
    }

    void Checkbox::SetLabel(const string& NewLabel)
    {
        LabelText = NewLabel;
        if (LabelDrawable)
            LabelDrawable->setString(LabelText);
        UpdateSize();
    }

    void Checkbox::SetBoxSize(float Size)
    {
        BoxSize = Size;
        if (BoxDrawable)
            BoxDrawable->setSize({ BoxSize, BoxSize });
        UpdateSize();
    }

    void Checkbox::SetLabelOffset(float Offset)
    {
        LabelOffset = Offset;
        UpdateSize();
    }

    void Checkbox::SetUncheckedColor(color Color)
    {
        UncheckedColor = Color;
        UpdateVisualState();
    }

    void Checkbox::SetCheckedColor(color Color)
    {
        CheckedColor = Color;
        UpdateVisualState();
    }

    void Checkbox::SetHoverColor(color Color)
    {
        HoverColor = Color;
        UpdateVisualState();
    }

    void Checkbox::UpdateVisualState()
    {
        if (!BoxDrawable)
            return;

        color TargetColor;

        if (bPressed)
            TargetColor = bChecked ? UncheckedColor : CheckedColor;
        else if (bHovered)
            TargetColor = HoverColor;
        else if (bChecked)
            TargetColor = CheckedColor;
        else
            TargetColor = UncheckedColor;

        BoxDrawable->setFillColor(TargetColor);
    }

    void Checkbox::UpdateSize()
    {
        float TotalWidth = BoxSize + LabelOffset;
        float TotalHeight = BoxSize;

        if (LabelDrawable)
        {
            auto Bounds = LabelDrawable->getLocalBounds();
            TotalWidth += Bounds.size.x;
            TotalHeight = std::max(TotalHeight, Bounds.size.y);
        }

        SetSize({ TotalWidth, TotalHeight });
    }
}