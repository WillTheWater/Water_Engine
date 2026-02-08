// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "PostProcess/PPE/EmbeddedPPETest.h"
#include "PostProcess/EmbeddedShaders.h"
#include "Utility/Assert.h"

namespace we
{
	EmbeddedPPETest::EmbeddedPPETest()
	{
		TestShader = make_shared<shader>();
		VERIFY(TestShader->loadFromMemory(std::string(EmbeddedShader::GrayscaleFragment), shader::Type::Fragment));
	}

	void EmbeddedPPETest::Apply(const sf::Texture& Input, sf::RenderTarget& Output)
	{
		TestShader->setUniform("Source", shader::CurrentTexture);
		Output.draw(sprite(Input), TestShader.get());
	}
}