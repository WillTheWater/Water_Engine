// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/ResourceSubsystem.h"
#include "EngineConfig.h"
#include "Utility/Log.h"

namespace we
{
	RenderSubsystem::RenderSubsystem()
		: RenderTarget{vec2u(EC.WindowSize)}
		, Background{EC.WindowSize}
	{
		Initialize();
	}

	void RenderSubsystem::Initialize()
	{
		RenderTarget.setSmooth(EC.SetRenderSmooth);
		DefaultBackground = Asset().LoadTexture(EC.DefaultBackground);
		Background.setTexture(DefaultBackground.get());
	}

	void RenderSubsystem::Draw(const drawable& RenderObject)
	{
		RenderTarget.draw(RenderObject);
	}

	void RenderSubsystem::SetRenderView(const view& RenderView)
	{
		RenderTarget.setView(RenderView);
	}

	void RenderSubsystem::ResetRenderView()
	{
		SetRenderView(RenderTarget.getDefaultView());
	}

	const view RenderSubsystem::ConstrainView(vec2u WindowSize)
	{
		float targetRatio = static_cast<float>(EC.AspectRatio.x) / static_cast<float>(EC.AspectRatio.y);
		float windowRatio = static_cast<float>(WindowSize.x) / static_cast<float>(WindowSize.y);

		float vWidth = 1.0f, vHeight = 1.0f, vPosX = 0.0f, vPosY = 0.0f;

		if (windowRatio > targetRatio) {
			vWidth = targetRatio / windowRatio;
			vPosX = (1.0f - vWidth) / 2.0f;
		}
		else {
			vHeight = windowRatio / targetRatio;
			vPosY = (1.0f - vHeight) / 2.0f;
		}

		view NewView;
		NewView.setSize(vec2f(RenderTarget.getSize()));
		NewView.setCenter(vec2f(RenderTarget.getSize()).componentWiseDiv({ 2, 2 }));

		NewView.setViewport(rectf({ vPosX, vPosY }, { vWidth, vHeight }));

		return NewView;
	}

	void RenderSubsystem::StartRender()
	{
		RenderTarget.clear(color::Black);

		Draw(Background);
	}

	const texture& RenderSubsystem::FinishRender()
	{
		RenderTarget.display();
		return RenderTarget.getTexture();
	}
}