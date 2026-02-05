// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Utility/CoreMinimal.h"

namespace we
{
	struct EngineSubsystem;
	class GameWindow;
	class Widget;

	class Widget : public std::enable_shared_from_this<Widget>
	{
	public:
		explicit Widget(EngineSubsystem& InSubsystem);
		virtual ~Widget() = default;

		virtual void Update(float DeltaTime) {}
		virtual void Render(GameWindow& Window) {}

	public:
		void SetLocalOffset(const vec2f& Offset) { LocalOffset += Offset; MarkDirty(); }
		vec2f GetLocalOffset() const { return LocalOffset; }

		void SetLocalRotation(angle Angle) { LocalRotation = Angle; MarkDirty(); }
		angle GetLocalRotation() const { return LocalRotation; }

		void SetLocalScale(const vec2f& InScale) { LocalScale = InScale; MarkDirty(); }
		vec2f GetLocalScale() const { return LocalScale; }

		vec2f GetWorldPosition() const;
		angle GetWorldRotation() const;
		vec2f GetWorldScale() const;

		void SetSize(const vec2f& InSize) { Size = InSize; }
		vec2f GetSize() const { return Size; }

		void SetParent(Widget* InParent) { Parent = InParent; MarkDirty(); }
		Widget* GetParent() const { return Parent; }

		void SetVisible(bool bInVisible) { bVisible = bInVisible; }
		bool IsVisible() const { return bVisible; }

		bool Contains(const vec2f& WorldPoint) const;

	protected:
		EngineSubsystem& Subsystem;

		vec2f LocalOffset{ 0, 0 };
		angle LocalRotation{};
		vec2f LocalScale{ 1, 1 };
		vec2f Size{ 100, 50 };

		Widget* Parent = nullptr;
		bool bVisible = true;

		mutable bool bDirty = true;
		mutable vec2f CachedWorldPosition;
		mutable angle CachedWorldRotation;
		mutable vec2f CachedWorldScale;

	private:
		void MarkDirty() { bDirty = true; }
		void UpdateCache() const;
	};
}