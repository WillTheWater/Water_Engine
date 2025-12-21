#include "Widgets/GameHUD.h"
#include "Framework/Renderer.h"

namespace we
{
	GameplayHUD::GameplayHUD()
		: PlayerHealth{}
	{
	}

	void GameplayHUD::Render(Renderer& GameRenderer)
	{
		PlayerHealth.NativeRender(GameRenderer);
	}

	void GameplayHUD::Tick(float DeltaTime)
	{
	}

	void GameplayHUD::Initialize(Renderer& GameRenderer)
	{
		PlayerHealth.UpdateFillPercentage(75, 100);
		auto WindowSize = GameRenderer.GetViewportSize();
		PlayerHealth.SetWidgetPosition({ 140.f, WindowSize.y - PlayerHealth.GetBarSize().y});
	}
}