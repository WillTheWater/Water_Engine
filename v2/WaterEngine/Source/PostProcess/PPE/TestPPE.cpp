// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include <SFML/Graphics/Sprite.hpp>

#include "Subsystem/AsyncResourceSubsystem.h"
#include "PostProcess/PPE/TestPPE.h"
#include "Utility/Assert.h"

namespace we
{
	PPETest::PPETest()
	{
		TestShader = AsyncAsset().LoadShader("Shaders/Test.frag", shader::Type::Fragment);
		VERIFY(TestShader != nullptr);
	}

	void PPETest::Apply(const sf::Texture& Input, sf::RenderTarget& Output)
	{
		TestShader.get()->setUniform("Source", shader::CurrentTexture);
		Output.draw(sprite(Input), TestShader.get());
	}
}