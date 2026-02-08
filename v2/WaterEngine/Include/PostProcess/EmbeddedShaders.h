// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

namespace we::EmbeddedShader
{
    // Post-processing shaders
    inline constexpr stringView InvertFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec4 col = texture2D(Source, uv);
            gl_FragColor = vec4(1.0 - col.rgb, col.a);
        }
    )";

    inline constexpr stringView GrayscaleFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec4 col = texture2D(Source, uv);
            float gray = dot(col.rgb, vec3(0.299, 0.587, 0.114));
            gl_FragColor = vec4(vec3(gray), col.a);
        }
    )";

    inline constexpr stringView SepiaFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec4 col = texture2D(Source, uv);
            vec3 sepia = vec3(
                dot(col.rgb, vec3(0.393, 0.769, 0.189)),
                dot(col.rgb, vec3(0.349, 0.686, 0.168)),
                dot(col.rgb, vec3(0.272, 0.534, 0.131))
            );
            gl_FragColor = vec4(sepia, col.a);
        }
    )";
}