// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once
#include "Utility/CoreMinimal.h"

namespace we
{
	class Widget;
	class GameWindow;

	class GUISubsystem
	{
	public:
		explicit GUISubsystem(GameWindow& Window);
		~GUISubsystem();

		void ProcessEvent(const sf::Event& Event);
		void Update(float DeltaTime);
		void Render();

		template<typename T, typename... Args>
		shared<T> CreateWidget(Args&&... args);

		void DestroyWidget(Widget* InWidget);
		void Clear();

		bool IsMousePressed() const { return bMousePressed; }

	private:
		GameWindow& Window;
		list<shared<Widget>> Widgets;
		list<weak<Widget>> RootWidgets;
		bool bMousePressed = false;

	private:
		void HandleEvent(const sf::Event::MouseButtonPressed&);
		void HandleEvent(const sf::Event::MouseButtonReleased&);
		void HandleEvent(const sf::Event::JoystickButtonPressed&);
		void HandleEvent(const sf::Event::JoystickButtonReleased&);
		void HandleEvent(const auto&) {}
	};

	template<typename T, typename ...Args>
	inline shared<T> GUISubsystem::CreateWidget(Args && ...args)
	{
		static_assert(std::is_base_of_v<Widget, T>, "T must derive from Widget");

		auto Widget = std::make_shared<T>(std::forward<Args>(args)...);
		Widgets.push_back(Widget);
		return Widget;
	}
}