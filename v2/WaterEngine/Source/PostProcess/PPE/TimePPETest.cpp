// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "PostProcess/PPE/TimePPETest.h"
#include "Subsystem/ResourceSubsystem.h"
#include "PostProcess/EmbeddedShaders.h"
#include "Utility/Assert.h"

namespace we
{
	TimePPETest::TimePPETest()
	{
		TestShader = LoadAsset().LoadShader("Shaders/TimeTest.frag", shader::Type::Fragment);
		VERIFY(TestShader != nullptr);
	}

	void TimePPETest::Apply(const texture& Input, renderTarget& Output)
	{
		float Elapsed = Time.getElapsedTime().asSeconds();
		TestShader->setUniform("Time", Elapsed);
		TestShader->setUniform("Source", shader::CurrentTexture);
		Output.draw(sprite(Input), TestShader.get());
	}
}