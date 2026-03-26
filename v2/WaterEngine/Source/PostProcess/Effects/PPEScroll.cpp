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

    void PPEScroll::Update(float DeltaTime)
    {
        ElapsedTime += DeltaTime;
    }

    void PPEScroll::Apply(const texture& Input, renderTarget& Output)
    {
        ScrollShader->setUniform("Time", ElapsedTime);
        Output.draw(sprite(Input), ScrollShader.get());
    }
}
