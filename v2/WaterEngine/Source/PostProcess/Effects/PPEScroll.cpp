// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "PostProcess/Effects/PPEScroll.h"
#include "PostProcess/Shaders/EmbeddedShaders.h"
#include "Utility/Assert.h"

namespace we
{
    PPEScroll::PPEScroll()
    {
        ScrollShader = make_shared<shader>();
        VERIFY(ScrollShader->loadFromMemory(
            string(EmbeddedShader::DefaultVertex), 
            string(EmbeddedShader::ScrollFragment)
        ));
        ScrollShader->setUniform("Source", shader::CurrentTexture);
    }

    void PPEScroll::Apply(const texture& Input, renderTarget& Output)
    {
        float Elapsed = Time.getElapsedTime().asSeconds();
        ScrollShader->setUniform("Time", Elapsed);
        Output.draw(sprite(Input), ScrollShader.get());
    }
}
