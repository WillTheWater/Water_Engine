// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Interface/PostProcess/IPostProcess.h"

namespace we
{
	class PPEGrayscale : public IPostProcess
	{
	public:
		PPEGrayscale();

		virtual void Apply(const texture& Input, renderTarget& Output) override;

	private:
		shared<shader> GrayscaleShader;
	};
}