// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "PostProcess/IPostProcess.h"

namespace we
{
	class RenderSubsystem
	{
	public:
		RenderSubsystem();

		void Draw(const drawable& RenderObject);

		void SetRenderView(const view& RenderView);
		vec2f GetRenderSize() const { return vec2f(RenderTarget.getSize()); }
		void ResetRenderView();
	
	private:
		renderTexture RenderTarget;
		renderTexture PostProcessTarget;
		vector<unique<IPostProcess>> PostProcessEffects;

	private:
		friend class WaterEngine;

		void Initialize();
		const view ConstrainView(vec2u WindowSize);
		void StartRender();
		const texture& FinishRender();
	};
}