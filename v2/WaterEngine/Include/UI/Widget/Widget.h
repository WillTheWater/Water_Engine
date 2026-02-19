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

		virtual void Update(float DeltaTime) {}
		virtual void CollectRenderDepths(vector<RenderDepth>& OutDepths) const {}

		// Input handling - override in derived widgets
		virtual void OnDrag(const vec2f& MousePosition) {}  // Called when mouse moves while pressed
		virtual void OnPressed(const vec2f& MousePosition) {}  // Called when mouse is pressed on widget
		virtual void OnReleased(const vec2f& MousePosition) {}  // Called when mouse is released

		Delegate<> OnClicked;
		Delegate<> OnFocusGained;  // Keyboard/gamepad navigation focus
		Delegate<> OnFocusLost;
		Delegate<> OnHoverGained;  // Mouse hover
		Delegate<> OnHoverLost;

		void SetAnchorPosition(Anchor ScreenAnchor, Anchor WidgetAnchor, vec2f Offset = vec2f{ 0.f, 0.f });
		void SetLocalOffset(const vec2f& Offset);
		vec2f GetLocalOffset() const { return LocalOffset; }

		void SetLocalScale(const vec2f& InScale);
		vec2f GetLocalScale() const { return LocalScale; }
		vec2f GetWorldScale() const;

		void SetSize(const vec2f& InSize);
		vec2f GetSize() const { return Size; }
		void SetAutoSize(bool bInEnabled, float Padding = 0.f);
		void CalculateAutoSize();

		vec2f GetWorldPosition() const;
		vec2f GetOrigin() const;

		virtual void AddChild(shared<Widget> Child, Anchor InTargetAnchor = Anchor::TopLeft, Anchor InWidgetAnchor = Anchor::TopLeft, vec2f InOffset = vec2f{ 0, 0 });
		void RemoveChild(Widget* Child);
		void SetParent(Widget* InParent);
		void DetachFromParent();
		Widget* GetParent() const { return Parent; }
		const vector<weak<Widget>>& GetChildren() const { return Children; }

		void SetVisible(bool bInVisible);
		void Show() { SetVisible(true); }
		void Hide() { SetVisible(false); }
		bool IsVisible() const { return bVisible; }

		virtual bool Contains(const vec2f& ScreenPoint) const;
		shared<Widget> FindDeepestChildAt(const vec2f& ScreenPoint);

		void SetFocusable(bool bFocusable) { bIsFocusable = bFocusable; }
		bool IsFocusable() const { return bIsFocusable; }

		// Override to define where the cursor should snap when navigating to this widget
		// Default is center of widget. CheckBox overrides to center, Slider to thumb position.
		virtual vec2f GetFocusPoint() const { return GetWorldPosition() + GetSize().componentWiseMul(GetWorldScale()) * 0.5f; }

		virtual float GetRenderDepth() const { return RenderDepthValue.Depth; }
		void SetRenderDepth(float Depth) { RenderDepthValue.Depth = Depth; }

		virtual void SetPressed(bool bPressed) {}

		void SetHoverSound(const string& Path) { HoverSound = Path; }
		void SetPressedSound(const string& Path) { PressedSound = Path; }
		void SetClickSound(const string& Path) { ClickSound = Path; }
		void SetFocusSound(const string& Path) { FocusSound = Path; }
		
		const string& GetHoverSound() const { return HoverSound; }
		const string& GetPressedSound() const { return PressedSound; }
		const string& GetClickSound() const { return ClickSound; }
		const string& GetFocusSound() const { return FocusSound; }

	protected:
		void MarkDirty();
		void UpdateCache() const;
		static vec2f GetAnchorPoint(const vec2f& InSize, Anchor InAnchor);

	private:
		vec2f LocalOffset{ 0, 0 };
		vec2f LocalScale{ 1, 1 };
		vec2f Size{ 100, 100 };

		Anchor TargetAnchor = Anchor::TopLeft;
		Anchor WidgetAnchor = Anchor::TopLeft;
		vec2f AnchorOffset{ 0, 0 };
		bool bUseAnchors = false;

		bool bAutoSize = false;
		float AutoSizePadding = 0.f;

		Widget* Parent = nullptr;

	protected:
		vector<weak<Widget>> Children;

	private:
		bool bVisible = true;
		bool bIsFocusable = false;
		RenderDepth RenderDepthValue{ nullptr, 0.f };

		string HoverSound;
		string PressedSound;
		string ClickSound;
		string FocusSound;

		mutable bool bDirty = true;
		mutable vec2f CachedWorldPosition;
		mutable vec2f CachedWorldScale;
		mutable vec2f CachedOrigin;
	};
}
