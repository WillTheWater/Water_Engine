// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Utility/Delegate.h"
#include "Framework/World/RenderTypes.h"

namespace we
{
	enum class Anchor
	{
		TopLeft, TopCenter, TopRight,
		CenterLeft, Center, CenterRight,
		BottomLeft, BottomCenter, BottomRight
	};

	class Widget : public std::enable_shared_from_this<Widget>
	{
	public:
		explicit Widget();
		virtual ~Widget() = default;

		// Lifecycle
		virtual void Update(float DeltaTime) {}

		// Rendering - collect drawables with depth (override to add visual elements; containers do nothing)
		virtual void CollectRenderDepths(vector<RenderDepth>& OutDepths) const {}

		// Render depth for this widget (used for hit testing, focus navigation, and as base for drawables)
		virtual float GetRenderDepth() const { return RenderDepthValue.Depth; }
		void SetRenderDepth(float InDepth) { RenderDepthValue.Depth = InDepth; }

		// Input delegates
		Delegate<> OnClicked;
		Delegate<> OnFocusGained;
		Delegate<> OnFocusLost;

		// Positioning
		void SetAnchorPosition(Anchor ScreenAnchor, Anchor WidgetAnchor, vec2f Offset = vec2f{ 0.f, 0.f });
		void SetLocalOffset(const vec2f& Offset);
		vec2f GetLocalOffset() const { return LocalOffset; }

		// Scale
		void SetLocalScale(const vec2f& InScale);
		vec2f GetLocalScale() const { return LocalScale; }
		vec2f GetWorldScale() const;

		// Size
		void SetSize(const vec2f& InSize);
		vec2f GetSize() const { return Size; }
		void SetAutoSize(bool bEnabled, float Padding = 0.f);
		void CalculateAutoSize();

		// World space
		vec2f GetWorldPosition() const;
		vec2f GetOrigin() const;

		// Hierarchy
		void AddChild(shared<Widget> Child, Anchor InTargetAnchor = Anchor::TopLeft, Anchor InWidgetAnchor = Anchor::TopLeft, vec2f InOffset = vec2f{ 0, 0 });
		void RemoveChild(Widget* Child);
		void SetParent(Widget* InParent);
		void DetachFromParent();
		Widget* GetParent() const { return Parent; }
		const vector<weak<Widget>>& GetChildren() const { return Children; }

		// Visibility
		void SetVisible(bool bInVisible) { bVisible = bInVisible; }
		bool IsVisible() const { return bVisible; }

		// Hit testing
		bool Contains(const vec2f& ScreenPoint) const;
		shared<Widget> FindDeepestChildAt(const vec2f& ScreenPoint);

		// Focus
		void SetFocusable(bool bFocusable) { bIsFocusable = bFocusable; }
		bool IsFocusable() const { return bIsFocusable; }

	protected:
		void MarkDirty();
		void UpdateCache() const;
		static vec2f GetAnchorPoint(const vec2f& InSize, Anchor InAnchor);

	private:
		// Transform
		vec2f LocalOffset{ 0, 0 };
		vec2f LocalScale{ 1, 1 };
		vec2f Size{ 100, 100 };

		// Anchor data
		Anchor TargetAnchor = Anchor::TopLeft;
		Anchor WidgetAnchor = Anchor::TopLeft;
		vec2f AnchorOffset{ 0, 0 };
		bool bUseAnchors = false;

		// Auto-size
		bool bAutoSize = false;
		float AutoSizePadding = 0.f;

		// Hierarchy
		Widget* Parent = nullptr;
		vector<weak<Widget>> Children;

		// State
		bool bVisible = true;
		bool bIsFocusable = false;
		RenderDepth RenderDepthValue{ nullptr, 0.f };

		// Cached world data
		mutable bool bDirty = true;
		mutable vec2f CachedWorldPosition;
		mutable vec2f CachedWorldScale;
		mutable vec2f CachedOrigin;
	};
}