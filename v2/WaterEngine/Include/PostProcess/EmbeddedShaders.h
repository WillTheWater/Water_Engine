// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

namespace we::EmbeddedShader
{
    // 1. Invert
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

    // 2. Grayscale
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

    // 3. Sepia
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

    // 4. Vignette
    inline constexpr stringView VignetteFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec4 col = texture2D(Source, uv);
            vec2 center = uv - vec2(0.5);
            float dist = length(center);
            float vignette = smoothstep(0.8, 0.2, dist);
            gl_FragColor = vec4(col.rgb * vignette, col.a);
        }
    )";

    // 5. Scanlines
    inline constexpr stringView ScanlinesFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec4 col = texture2D(Source, uv);
            float scanline = sin(uv.y * 800.0) * 0.1;
            gl_FragColor = vec4(col.rgb - scanline, col.a);
        }
    )";

    // 6. Pixelate
    inline constexpr stringView PixelateFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            float pixels = 64.0;
            vec2 newuv = floor(uv * pixels) / pixels;
            gl_FragColor = texture2D(Source, newuv);
        }
    )";

    // 7. Blur (simple box blur)
    inline constexpr stringView BlurFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec2 texel = vec2(1.0 / 800.0, 1.0 / 600.0);
            vec4 col = vec4(0.0);
            for(int x = -2; x <= 2; x++)
                for(int y = -2; y <= 2; y++)
                    col += texture2D(Source, uv + vec2(x, y) * texel);
            gl_FragColor = col / 25.0;
        }
    )";

    // 8. Edge Detection (Sobel)
    inline constexpr stringView EdgeDetectFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec2 texel = vec2(1.0 / 800.0, 1.0 / 600.0);
            vec4 col = texture2D(Source, uv);
            vec4 left = texture2D(Source, uv - vec2(texel.x, 0.0));
            vec4 right = texture2D(Source, uv + vec2(texel.x, 0.0));
            vec4 top = texture2D(Source, uv - vec2(0.0, texel.y));
            vec4 bottom = texture2D(Source, uv + vec2(0.0, texel.y));
            vec3 edge = abs(left.rgb - right.rgb) + abs(top.rgb - bottom.rgb);
            gl_FragColor = vec4(edge, col.a);
        }
    )";

    // 9. CRT Curve
    inline constexpr stringView CRTFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec2 centered = uv - 0.5;
            float dist = dot(centered, centered) * 0.5;
            vec2 curved = uv + centered * dist;
            if(curved.x < 0.0 || curved.x > 1.0 || curved.y < 0.0 || curved.y > 1.0)
                gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
            else
                gl_FragColor = texture2D(Source, curved);
        }
    )";

    // 10. Night Vision
    inline constexpr stringView NightVisionFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec4 col = texture2D(Source, uv);
            float gray = dot(col.rgb, vec3(0.299, 0.587, 0.114));
            vec3 green = vec3(0.0, gray * 1.5, 0.0);
            float noise = fract(sin(dot(uv, vec2(12.9898, 78.233))) * 43758.5453);
            gl_FragColor = vec4(green + noise * 0.1, col.a);
        }
    )";

    // 11. Thermal Vision
    inline constexpr stringView ThermalFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec4 col = texture2D(Source, uv);
            float gray = dot(col.rgb, vec3(0.299, 0.587, 0.114));
            vec3 thermal;
            thermal.r = smoothstep(0.0, 0.5, gray);
            thermal.g = smoothstep(0.3, 0.8, gray);
            thermal.b = smoothstep(0.5, 1.0, gray);
            gl_FragColor = vec4(thermal, col.a);
        }
    )";

    // 12. Posterize
    inline constexpr stringView PosterizeFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec4 col = texture2D(Source, uv);
            float levels = 4.0;
            gl_FragColor = vec4(floor(col.rgb * levels) / levels, col.a);
        }
    )";

    // 13. Chromatic Aberration
    inline constexpr stringView ChromaticFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            float offset = 0.01;
            float r = texture2D(Source, uv + vec2(offset, 0.0)).r;
            float g = texture2D(Source, uv).g;
            float b = texture2D(Source, uv - vec2(offset, 0.0)).b;
            gl_FragColor = vec4(r, g, b, texture2D(Source, uv).a);
        }
    )";

    // 14. Wave Distortion
    inline constexpr stringView WaveFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            uv.x += sin(uv.y * 20.0) * 0.02;
            gl_FragColor = texture2D(Source, uv);
        }
    )";

    // 15. Outline
    inline constexpr stringView OutlineFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec2 texel = vec2(1.0 / 800.0, 1.0 / 600.0);
            float alpha = 0.0;
            for(int x = -1; x <= 1; x++)
                for(int y = -1; y <= 1; y++)
                    alpha += texture2D(Source, uv + vec2(x, y) * texel).a;
            alpha = alpha / 9.0;
            vec4 col = texture2D(Source, uv);
            if(col.a < 0.5 && alpha > 0.5)
                gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
            else
                gl_FragColor = col;
        }
    )";

    // 16. Brightness/Contrast
    inline constexpr stringView BrightnessContrastFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec4 col = texture2D(Source, uv);
            float brightness = 0.2;
            float contrast = 1.5;
            vec3 result = (col.rgb - 0.5) * contrast + 0.5 + brightness;
            gl_FragColor = vec4(result, col.a);
        }
    )";

    // 17. Saturation
    inline constexpr stringView SaturationFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec4 col = texture2D(Source, uv);
            float gray = dot(col.rgb, vec3(0.299, 0.587, 0.114));
            float saturation = 2.0;
            gl_FragColor = vec4(mix(vec3(gray), col.rgb, saturation), col.a);
        }
    )";

    // 18. Tint
    inline constexpr stringView TintFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec4 col = texture2D(Source, uv);
            vec3 tint = vec3(0.2, 0.4, 0.8);
            gl_FragColor = vec4(col.rgb * tint, col.a);
        }
    )";

    // 19. Film Grain
    inline constexpr stringView FilmGrainFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec4 col = texture2D(Source, uv);
            float grain = fract(sin(dot(uv * 1000.0, vec2(12.9898, 78.233))) * 43758.5453);
            gl_FragColor = vec4(col.rgb + (grain - 0.5) * 0.2, col.a);
        }
    )";

    // 20. Dream/Blur Vision
    inline constexpr stringView DreamFragment = R"(
        #version 130
        uniform sampler2D Source;
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec4 col = vec4(0.0);
            float radius = 0.005;
            for(float x = -2.0; x <= 2.0; x += 1.0)
                for(float y = -2.0; y <= 2.0; y += 1.0)
                    col += texture2D(Source, uv + vec2(x, y) * radius);
            col /= 25.0;
            col.rgb = mix(col.rgb, vec3(0.8, 0.6, 0.9), 0.3);
            gl_FragColor = col;
        }
    )";

    // 21. DownSample
    inline constexpr stringView DownSample = R"(
        #version 130
        uniform sampler2D Source;
        uniform vec2 TexSize;
        
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec4 color = texture2D(Source, uv);
            
            vec2 off = TexSize * 0.5;
            
            color += texture2D(Source, uv + vec2(off.x, 0.0));
            color += texture2D(Source, uv - vec2(off.x, 0.0));
            color += texture2D(Source, uv + vec2(0.0, off.y));
            color += texture2D(Source, uv - vec2(0.0, off.y));
            color += texture2D(Source, uv + vec2(off.x, off.y));
            color += texture2D(Source, uv - vec2(off.x, off.y));
            color += texture2D(Source, uv + vec2(off.x, -off.y));
            color += texture2D(Source, uv - vec2(off.x, -off.y));
            
            gl_FragColor = color / 9.0;
        }
    )";

    // 22. Blur - fixed with clamping
    inline constexpr stringView Blur = R"(
        #version 130
        uniform sampler2D Source;
        uniform vec2 TexSize;
        
        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec4 color = texture2D(Source, uv) * 0.2270270270;
            
            vec2 off = TexSize;
            
            // Clamp UVs to prevent sampling outside texture (causes reflection)
            vec2 sample;
            
            sample = clamp(uv + off * 1.0, vec2(0.0), vec2(1.0));
            color += texture2D(Source, sample) * 0.1945945946;
            
            sample = clamp(uv - off * 1.0, vec2(0.0), vec2(1.0));
            color += texture2D(Source, sample) * 0.1945945946;
            
            sample = clamp(uv + off * 2.0, vec2(0.0), vec2(1.0));
            color += texture2D(Source, sample) * 0.1216216216;
            
            sample = clamp(uv - off * 2.0, vec2(0.0), vec2(1.0));
            color += texture2D(Source, sample) * 0.1216216216;
            
            sample = clamp(uv + off * 3.0, vec2(0.0), vec2(1.0));
            color += texture2D(Source, sample) * 0.0540540541;
            
            sample = clamp(uv - off * 3.0, vec2(0.0), vec2(1.0));
            color += texture2D(Source, sample) * 0.0540540541;
            
            sample = clamp(uv + off * 4.0, vec2(0.0), vec2(1.0));
            color += texture2D(Source, sample) * 0.0162162162;
            
            sample = clamp(uv - off * 4.0, vec2(0.0), vec2(1.0));
            color += texture2D(Source, sample) * 0.0162162162;
            
            gl_FragColor = color;
        }
    )";
    
    // 23. Additive
    inline constexpr stringView Additive = R"(
        #version 130
        uniform sampler2D Source;
        uniform sampler2D BloomTex;

        const float BLOOM_STRENGTH = 0.18;

        void main()
        {
            vec2 uv = gl_TexCoord[0].xy;
            vec2 bloomUV = vec2(uv.x, 1.0 - uv.y);
            
            vec4 sourceColor = texture2D(Source, uv);
            vec4 bloomColor  = texture2D(BloomTex, bloomUV);
            
            gl_FragColor = sourceColor + bloomColor * BLOOM_STRENGTH;
        }
    )";
    
    // DefaultVerts
    inline constexpr stringView DefaultVerts = R"(
        #version 130

        void main()
        {
            gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
            gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
            gl_FrontColor = gl_Color;
        }
    )";
}