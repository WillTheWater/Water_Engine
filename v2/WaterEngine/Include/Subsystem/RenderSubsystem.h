// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Core/EngineConfig.h"
#include "Interface/ICamera.h"
#include "Interface/IPostProcessEffect.h"
#include <optional>

namespace we
{
	enum class ERenderLayer { World, ScreenUI, WorldUI, Cursor };
	enum class EViewSpace { World, Screen };



	// =========================================================================
	// RenderSubsystem - Multi-layer rendering with post-processing
	// =========================================================================
	class RenderSubsystem
	{
	public:
		// Initialize with config and window (for cursor sizing & final output)
		RenderSubsystem(const EngineConfig::RenderConfig& Config, sf::Window& Window);
		~RenderSubsystem();

		// Frame management
		void BeginFrame();
		void EndFrame();

		// Drawing
		void Draw(const drawable& Object, ERenderLayer Layer);
		void Draw(const drawable& Object, ERenderLayer Layer, EViewSpace ViewSpace);

		// Camera view for world layers
		void SetWorldView(const std::optional<CameraView>& View = std::nullopt);

		// Final output
		sprite FinishComposite();  // Returns final composited frame with letterboxing
		void PresentCursor();      // Draw cursor separately (1:1 with window)

		// For Editor viewport
		const texture& GetWorldTexture() const;

		// Coordinate mapping (window pixel to world coords)
		vec2f MapPixelToCoords(vec2i PixelPos) const;

		// Post-processing
		void AddPostProcessEffect(ERenderLayer Layer, unique<IPostProcessEffect> Effect);

	private:
		// Main render targets (stored directly like old code)
		renderTexture WorldRenderTarget;
		renderTexture ScreenUIRenderTarget;
		renderTexture WorldUIRenderTarget;
		renderTexture CursorRenderTarget;
		renderTexture CompositeTarget;

		// Post-process targets
		renderTexture WorldPostProcessTarget;
		renderTexture ScreenUIPostProcessTarget;
		renderTexture WorldUIPostProcessTarget;
		renderTexture CursorPostProcessTarget;

		// Post-process effects per layer
		vector<unique<IPostProcessEffect>> WorldPostProcessEffects;
		vector<unique<IPostProcessEffect>> ScreenUIPostProcessEffects;
		vector<unique<IPostProcessEffect>> WorldUIPostProcessEffects;
		vector<unique<IPostProcessEffect>> CursorPostProcessEffects;

		// State
		vec2u RenderResolution;
		vec2u WindowSize;
		sf::Window& RenderWindow;
		view CurrentWorldView;
		view LetterboxView;

		// Helpers - using reference_wrapper to avoid raw pointers
		renderTexture& GetMainTargetForLayer(ERenderLayer Layer);
		renderTexture& GetPostProcessTargetForLayer(ERenderLayer Layer);
		vector<unique<IPostProcessEffect>>& GetEffectsForLayer(ERenderLayer Layer);
		
		void ClearRenderTargets();
		void CompositeLayers();
		void UpdateLetterboxView(float Scale, float PosX, float PosY);
		const texture& GetLayerTexture(ERenderLayer Layer);
		renderTexture& ProcessPostEffects(renderTexture& Input, renderTexture& Output, 
			vector<unique<IPostProcessEffect>>& Effects);
	};
}
