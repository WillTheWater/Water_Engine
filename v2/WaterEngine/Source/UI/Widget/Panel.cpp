// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/Panel.h"
#include "Subsystem/ResourceSubsystem.h"
#include "Subsystem/GUISubsystem.h"
#include "Framework/GameWindow.h"
#include "Framework/EngineSubsystem.h"

namespace we
{
    Panel::Panel(
        EngineSubsystem& Subsystem,
        const list<shared<Widget>>& InChildren,
        rectf Margins,
        Anchor InTargetAnchor,
        Anchor InWidgetAnchor,
        const vec2f& InOffset
    )
        : Widget{ Subsystem }
    {
        // ------------------------------------------------------------
        // Compute content bounds from children (local space)
        // ------------------------------------------------------------

        float MinX = FLT_MAX, MinY = FLT_MAX;
        float MaxX = -FLT_MAX, MaxY = -FLT_MAX;

        for (const auto& Child : InChildren)
        {
            if (!Child)
                continue;

            // Attach child without altering its anchor setup
            Child->SetParent(this);
            Children.push_back(Child);

            const vec2f ChildPos = Child->AnchorOffset + Child->LocalOffset;
            const vec2f ChildScale = Child->GetWorldScale();
            const vec2f ChildSize = Child->GetSize().componentWiseMul(ChildScale);
            const vec2f ChildOrigin = Child->GetOrigin().componentWiseMul(ChildScale);

            const float Left = ChildPos.x - ChildOrigin.x;
            const float Top = ChildPos.y - ChildOrigin.y;
            const float Right = Left + ChildSize.x;
            const float Bottom = Top + ChildSize.y;

            MinX = std::min(MinX, Left);
            MinY = std::min(MinY, Top);
            MaxX = std::max(MaxX, Right);
            MaxY = std::max(MaxY, Bottom);
        }

        // ------------------------------------------------------------
        // Margins
        // (position = { top, bottom }, size = { left, right })
        // ------------------------------------------------------------

        const float TopMargin = Margins.position.x;
        const float BottomMargin = Margins.position.y;
        const float LeftMargin = Margins.size.x;
        const float RightMargin = Margins.size.y;

        // ------------------------------------------------------------
        // Panel size = content size + margins
        // ------------------------------------------------------------

        const float ContentWidth = MaxX - MinX;
        const float ContentHeight = MaxY - MinY;

        const vec2f PanelSize = {
            ContentWidth + LeftMargin + RightMargin,
            ContentHeight + TopMargin + BottomMargin
        };

        SetSize(PanelSize);

        // ------------------------------------------------------------
        // Offset children so content starts at margins
        // ------------------------------------------------------------

        const vec2f ContentOffset = {
            LeftMargin - MinX,
            TopMargin - MinY
        };

        for (auto& WeakChild : Children)
        {
            if (auto Child = WeakChild.lock())
            {
                const vec2f NewAnchorOffset =
                    Child->AnchorOffset + ContentOffset;

                Child->SetAnchorPosition(
                    Anchor::TopLeft,
                    Child->WidgetAnchor,
                    NewAnchorOffset
                );
            }
        }

        // ------------------------------------------------------------
        // Anchor the panel itself
        // ------------------------------------------------------------

        SetAnchorPosition(
            InTargetAnchor,
            InWidgetAnchor,
            InOffset
        );

        // ------------------------------------------------------------
        // Background
        // ------------------------------------------------------------

        Background.emplace();
        Background->setSize(PanelSize);
        Background->setFillColor(color::White);
        Background->setOutlineColor(color::Black);
        Background->setOutlineThickness(2.f);
    }


	Panel::Panel(EngineSubsystem& Subsystem, const string& TexturePath)
		: Widget{ Subsystem }
	{
		if (TexturePath.empty()) return;

		auto Tex = Asset().LoadTexture(TexturePath);
		if (!Tex) return;

		vec2f TexSize = vec2f(Tex->getSize());
		SetSize(TexSize);

		Background.emplace();
		Background->setSize(TexSize);
		Background->setTexture(&*Tex);
	}

	void Panel::Update(float DeltaTime)
	{
	}

	void Panel::Render(GameWindow& Window)
	{
		if (!IsVisible()) return;

		if (Background)
		{
			Background->setPosition(GetWorldPosition());
			Background->setScale(GetWorldScale());
			Window.draw(*Background);
		}

		Widget::Render(Window);
	}
}