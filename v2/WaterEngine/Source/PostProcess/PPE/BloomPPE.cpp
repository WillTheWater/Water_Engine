// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "PostProcess/PPE/BloomPPE.h"
#include "PostProcess/EmbeddedShaders.h"
#include "EngineConfig.h"
#include "Utility/Assert.h"

namespace we
{
	BloomPPE::BloomPPE()
	{
		DownSample = make_shared<shader>();
		Blur = make_shared<shader>();
		Additive = make_shared<shader>();

		const string VertexShader = string(EmbeddedShader::DefaultVerts);

		VERIFY(DownSample->loadFromMemory(VertexShader,string(EmbeddedShader::DownSample)));
		VERIFY(Blur->loadFromMemory(VertexShader,string(EmbeddedShader::Blur)));
		VERIFY(Additive->loadFromMemory(VertexShader,string(EmbeddedShader::Additive)));

		for (auto& T : Targets)
		{
			VERIFY(T.resize(vec2u(EC.WindowSize / 2.f)));
		}
	}

	void BloomPPE::Apply(const sf::Texture& Input, sf::RenderTarget& Output)
	{
		// 1. DownSample
		DownSample.get()->setUniform("Source", Input);
		DownSample.get()->setUniform("TexSize", vec2f(1.f / Input.getSize().x, 1.f / Input.getSize().y));
		Render(*DownSample.get(), Targets[0]);

		// 2. Blur (Vertical)
		Blur.get()->setUniform("Source", Targets[0].getTexture());
		Blur.get()->setUniform("TexSize", vec2f(0, 1.f / Targets[0].getSize().y));
		Render(*Blur.get(), Targets[1]);

		// 3. Blur (Horizontal)
		Blur.get()->setUniform("Source", Targets[1].getTexture());
		Blur.get()->setUniform("TexSize", vec2f(1.f / Targets[1].getSize().x, 0));
		Render(*Blur.get(), Targets[0]);

		// 4. Additive
		Additive.get()->setUniform("Source", shader::CurrentTexture);
		Additive.get()->setUniform("BloomTex", Targets[0].getTexture());

		Output.draw(sprite(Input), Additive.get());
	}

	void BloomPPE::Render(const shader& Shader, renderTexture& Output)
	{
		const vec2f Size(Output.getSize());

		const sf::Vertex Quad[] =
		{
			{{0, 0},       color::White, {0, 0}},
			{{Size.x, 0},  color::White, {1, 0}},
			{{0, Size.y},  color::White, {0, 1}},

			{{Size.x, 0},  color::White, {1, 0}},
			{{Size.x, Size.y}, color::White, {1, 1}},
			{{0, Size.y},  color::White, {0, 1}}
		};

		sf::RenderStates State(&Shader);
		State.blendMode = sf::BlendNone;

		Output.draw(Quad, 6, sf::PrimitiveType::Triangles, State);
		Output.display();
	}
}