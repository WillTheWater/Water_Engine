// =============================================================================
// Water Engine v2.1.1
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "PostProcess/Effects/PPEGrayscale.h"
#include "PostProcess/Shaders/EmbeddedShaders.h"
#include "Utility/Assert.h"

namespace we
{
	PPEGrayscale::PPEGrayscale()
	{
		GrayscaleShader = make_shared<shader>();
		VERIFY(GrayscaleShader->loadFromMemory(string(EmbeddedShader::GrayscaleFragment), shader::Type::Fragment));
	}

	void PPEGrayscale::Apply(const texture& Input, renderTarget& Output)
	{
		GrayscaleShader->setUniform("Source", shader::CurrentTexture);
		Output.draw(sprite(Input), GrayscaleShader.get());
	}
}