// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "PostProcess/IPostProcess.h"

namespace we
{
	class TimePPETest : public IPostProcess
	{
	public:
		TimePPETest();

		virtual void Apply(const texture& Input, renderTarget& Output) override;

	private:
		shared<shader> TestShader;
		clock Time;
	};
}