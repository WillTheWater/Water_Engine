// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "EngineConfig.h"
#include "Framework/WaterEngine.h"
#include "Utility/Log.h"

namespace we
{
	WaterEngine::WaterEngine()
	{
		if (EC.DisableSFMLLogs) { sf::err().rdbuf(nullptr); }
		Window = make_unique<GameWindow>();
		Window->OnResize = [this](vec2u newSize)
			{
				const auto CorrectedView = Subsystem.Render.ConstrainView(Window->getSize());
				Window->setView(CorrectedView);
			};
	}

	void WaterEngine::ProcessEvents()
	{
		Window->HandleEvents();
	}

	void WaterEngine::GlobalTick()
	{
		Subsystem.Time.Tick();
	}

	void WaterEngine::Render()
	{
		Window->clear();

		Subsystem.Render.StartRender();
		Window->draw(sprite(Subsystem.Render.FinishRender()));

		Window->display();
	}

	bool WaterEngine::IsRunning() const
	{
		return Window->isOpen();
	}

	bool we::WaterEngine::HasFocus() const
	{
		return Window->hasFocus();
	}

	void WaterEngine::ConstrainRender()
	{
		Subsystem.Render.ConstrainView(Window->getSize());
	}
}