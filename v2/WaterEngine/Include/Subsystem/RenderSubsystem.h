// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"
#include "PostProcess/IPostProcess.h"

namespace we
{
	class RenderSubsystem
	{
	public:
		RenderSubsystem();

		void Draw(const drawable& RenderObject);

		void SetRenderView(const view& RenderView);
		void ResetRenderView();
	
	private:
		renderTexture RenderTarget;
		renderTexture PostProcessTarget;
		list<unique<IPostProcess>> PostProcessEffects;

	private:
		friend class WaterEngine;

		void Initialize();
		const view ConstrainView(vec2u WindowSize);
		void StartRender();
		const texture& FinishRender();
	};
}