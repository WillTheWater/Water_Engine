// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "PostProcess/Effects/PPEClouds.h"
#include "PostProcess/Shaders/EmbeddedShaders.h"
#include "Utility/Assert.h"

namespace we
{
    PPEClouds::PPEClouds()
    {
        CloudsShader = make_shared<shader>();
        VERIFY(CloudsShader->loadFromMemory(
            string(EmbeddedShader::DefaultVertex), 
            string(EmbeddedShader::CloudsFragment)
        ));
        CloudsShader->setUniform("Source", shader::CurrentTexture);
    }

    void PPEClouds::Update(float DeltaTime)
    {
        ElapsedTime += DeltaTime;
    }

    void PPEClouds::Apply(const texture& Input, renderTarget& Output)
    {
        CloudsShader->setUniform("Time", ElapsedTime);
        CloudsShader->setUniform("ScrollSpeedX", -0.005f);  // Horizontal scroll speed
        CloudsShader->setUniform("ScrollSpeedY", 0.0f);   // Vertical scroll speed (0 = none)
        Output.draw(sprite(Input), CloudsShader.get());
    }
}
