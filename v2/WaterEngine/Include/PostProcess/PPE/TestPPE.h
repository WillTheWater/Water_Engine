// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "PostProcess/IPostProcess.h"

namespace we
{
	class PPETest : public IPostProcess
	{
	public:
		PPETest();

		virtual void Apply(const texture& Input, renderTarget& Output) override;

	private:
		shared<shader> TestShader;
	};
}