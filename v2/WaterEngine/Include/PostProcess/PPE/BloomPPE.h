// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "PostProcess/IPostProcess.h"

namespace we
{
	class BloomPPE : public IPostProcess
	{
	public:
		BloomPPE();

		virtual void Apply(const texture& Input, renderTarget& Output) override;

	private:
		shared<shader> DownSample;
		shared<shader> Blur;
		shared<shader> Additive;
		array<renderTexture, 2> Targets;

	private:
		void Render(const shader& Shader, renderTexture& Output);
	};
}