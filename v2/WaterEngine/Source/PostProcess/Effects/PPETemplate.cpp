// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "PostProcess/Effects/PPETemplate.h"
#include "PostProcess/Shaders/EmbeddedShaders.h"
#include "Utility/Assert.h"

namespace we
{
	PPETemplate::PPETemplate()
	{
		TemplateShader = make_shared<shader>();
		VERIFY(TemplateShader->loadFromMemory(string(EmbeddedShader::BrightnessContrastFragment), shader::Type::Fragment));
	}

	void PPETemplate::Apply(const texture& Input, renderTarget& Output)
	{
		TemplateShader->setUniform("Source", shader::CurrentTexture);
		Output.draw(sprite(Input), TemplateShader.get());
	}
}