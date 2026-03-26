// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "PostProcess/Effects/PPEWave.h"
#include "PostProcess/Shaders/EmbeddedShaders.h"
#include "Utility/Assert.h"

namespace we
{
    PPEWave::PPEWave()
    {
        WaveShader = make_shared<shader>();
        VERIFY(WaveShader->loadFromMemory(
            string(EmbeddedShader::DefaultVertex), 
            string(EmbeddedShader::HorizontalWaveFragment)
        ));
        WaveShader->setUniform("Source", shader::CurrentTexture);
    }

    void PPEWave::Update(float DeltaTime)
    {
        ElapsedTime += DeltaTime;
    }

    void PPEWave::Apply(const texture& Input, renderTarget& Output)
    {
        WaveShader->setUniform("Time", ElapsedTime);
        Output.draw(sprite(Input), WaveShader.get());
    }
}
