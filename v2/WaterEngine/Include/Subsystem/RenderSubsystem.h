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
	class WindowSubsystem;
	enum class ERenderLayer { World, ScreenUI, WorldUI, Cursor };
	enum class EViewSpace { World, Screen };

	// =========================================================================
	// RenderSubsystem - Multi-layer rendering with post-processing
	// =========================================================================
	class RenderSubsystem
	{
	public:
		// Initialize with config and window (for cursor sizing & final output)
		RenderSubsystem(const EngineConfig::RenderConfig& Config, WindowSubsystem& Window);
		~RenderSubsystem();

		// Frame management
		void BeginFrame();
		void EndFrame();

		// Drawing
		void Draw(const drawable& Object, ERenderLayer Layer);
		void Draw(const drawable& Object, ERenderLayer Layer, EViewSpace ViewSpace);

		// Camera view for world layers
		void SetWorldView(const std::optional<CameraView>& View = std::nullopt);

		// Editor camera navigation - offset applied to default view
		void SetEditorCameraOffset(const vec2f& Offset) { EditorCameraOffset = Offset; }
		vec2f GetEditorCameraOffset() const { return EditorCameraOffset; }

		// Editor zoom
		void SetEditorZoom(float Zoom) { EditorZoom = Zoom; }
		float GetEditorZoom() const { return EditorZoom; }

		// Editor grid toggle
		void SetGridEnabled(bool bEnabled) { bGridEnabled = bEnabled; }
		bool IsGridEnabled() const { return bGridEnabled; }
		void SetGridOriginEnabled(bool bEnabled) { bGridOriginEnabled = bEnabled; }
		bool IsGridOriginEnabled() const { return bGridOriginEnabled; }

		// Editor grid colors
		void SetGridColor(const color& GridColor) { GridLineColor = GridColor; }
		void SetGridXAxisColor(const color& AxisColor) { GridXAxisColor = AxisColor; }
		void SetGridYAxisColor(const color& AxisColor) { GridYAxisColor = AxisColor; }
		
		color GetGridColor() const { return GridLineColor; }
		color GetGridXAxisColor() const { return GridXAxisColor; }
		color GetGridYAxisColor() const { return GridYAxisColor; }

		// Editor grid overlay - draws X/Y axes and grid lines
		void DrawEditorGrid(float GridSpacing = 100.0f, int Subdivisions = 4);

		// Final output
		sprite FinishComposite();  // Returns final composited frame with letterboxing
		void PresentCursor();      // Draw cursor separately (1:1 with window)

		// For Editor viewport
		const texture& GetWorldTexture() const;
		vec2u GetRenderResolution() const { return RenderResolution; }

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
		WindowSubsystem& RenderWindow;
		view CurrentWorldView;
		view LetterboxView;
		vec2f EditorCameraOffset;
		float EditorZoom = 1.0f;

		// Grid settings
		bool bGridEnabled = true;
		bool bGridOriginEnabled = true;
		color GridLineColor = color(255, 255, 255, 120);
		color GridXAxisColor = color(255, 80, 80);
		color GridYAxisColor = color(80, 255, 80);

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
