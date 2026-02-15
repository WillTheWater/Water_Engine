// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Core/CoreMinimal.h"

namespace we
{
	class Widget;
	class GameWindow;
	class CursorSubsystem;

	class GUISubsystem
	{
	public:
		GUISubsystem(GameWindow& Window, CursorSubsystem& InCursor);
		~GUISubsystem();

		void ProcessEvent(const sf::Event& Event);
		void Update(float DeltaTime);
		void Render();

		template<typename T, typename... Args>
		shared<T> CreateWidget(Args&&... args);

		void Clear();

		void EventToggleFullscreen();

	private:
		GameWindow& Window;
		CursorSubsystem& Cursor;
		vector<shared<Widget>> Widgets;
		weak<Widget> HoveredWidget;
		weak<Widget> PressedWidget;

		shared<Widget> FindWidgetAt(const vec2f& WorldPoint) const;
		void HandleEvent(const sf::Event::MouseMoved&);
		void HandleEvent(const sf::Event::MouseButtonPressed&);
		void HandleEvent(const sf::Event::MouseButtonReleased&);
		void HandleEvent(const sf::Event::JoystickButtonPressed&);
		void HandleEvent(const sf::Event::JoystickButtonReleased&);
		void HandleEvent(const auto&) {}
	};

	template<typename T, typename... Args>
	inline shared<T> GUISubsystem::CreateWidget(Args&&... args)
	{
		static_assert(std::is_base_of_v<Widget, T>, "T must derive from Widget");
		auto Widget = std::make_shared<T>(std::forward<Args>(args)...);
		Widgets.push_back(Widget);
		return Widget;
	}
}