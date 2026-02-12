// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Utility/CoreMinimal.h"

namespace we
{
	enum class Anchor
	{
		TopLeft, TopCenter, TopRight,
		CenterLeft, Center, CenterRight,
		BottomLeft, BottomCenter, BottomRight
	};

	struct EngineSubsystem;
	class GameWindow;

	class Widget : public std::enable_shared_from_this<Widget>
	{
	public:
		explicit Widget(EngineSubsystem& InSubsystem);
		virtual ~Widget() = default;

		virtual void Update(float DeltaTime) {}
		virtual void Render(GameWindow& Window);

		virtual void OnMouseEnter() {}
		virtual void OnMouseLeave() {}
		virtual bool OnMouseButtonPressed() { return false; }
		virtual void OnMouseButtonReleased() {}
		virtual void OnMouseMoved(const vec2f&) {}

		// Positioning
		void SetAnchorPosition(Anchor ScreenAnchor, Anchor WidgetAnchor, vec2f Offset = vec2f{0.f, 0.f});
		
		// Nesting
		virtual void AddChild(shared<Widget> Child, Anchor InTargetAnchor, Anchor InWidgetAnchor, vec2f InOffset = { 0,0 });
		virtual void RemoveChild(Widget* Child);
		virtual void RenderChildren(GameWindow& Window);

		// Transform
		void SetLocalOffset(const vec2f& Offset);
		vec2f GetLocalOffset() const;
		void SetLocalScale(const vec2f& InScale);
		vec2f GetLocalScale() const;

		// Size
		void SetSize(const vec2f& InSize);
		vec2f GetSize() const;
		void SetAutoSize(bool bEnabled, float Padding = 0.f);
		void CalculateAutoSize();

		// World space
		vec2f GetWorldPosition() const;
		vec2f GetWorldScale() const;
		vec2f GetOrigin() const;

		// Hierarchy
		void SetParent(Widget* InParent);
		void DetachFromParent();
		Widget* GetParent() const;

		// Visibility
		void SetVisible(bool bInVisible);
		bool IsVisible() const;

		// Z-Order
		uint GetZOrder() const;
		void SetZOrder(uint InZOrder);

		// Hit test
		bool Contains(const vec2f& WorldPoint) const;
		shared<Widget> FindDeepestChildAt(const vec2f& WorldPoint);

		EngineSubsystem& Subsystem;

		virtual void UpdateCache() const;
		void MarkDirty();
		// Cached world data
		list<weak<Widget>> Children;
		mutable bool bDirty = true;
		mutable vec2f CachedWorldPosition;
		mutable vec2f CachedWorldScale;
		mutable vec2f CachedOrigin;
		
		// Transform data
		vec2f LocalOffset{ 0, 0 };
		vec2f LocalScale{ 1, 1 };
		vec2f Size{ 50, 50 };
		bool bAutoSize = false;
		float AutoSizePadding = 0.f;

		// Anchor data
		Anchor TargetAnchor = Anchor::TopLeft;
		Anchor WidgetAnchor = Anchor::TopLeft;
		vec2f AnchorOffset{ 0, 0 };
		bool bUseAnchors = false;

		// Hierarchy
		Widget* Parent = nullptr;
		bool bVisible = true;
		uint ZOrder = 0;

		static vec2f GetAnchorPoint(const vec2f& InSize, Anchor InAnchor);
		vec2f CalculateAnchorPosition() const;
	};
}