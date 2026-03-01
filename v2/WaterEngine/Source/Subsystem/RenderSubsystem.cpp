// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Subsystem/RenderSubsystem.h"
#include "Subsystem/WindowSubsystem.h"
#include "Utility/Log.h"
#include "Utility/Assert.h"

namespace we
{
	RenderSubsystem::RenderSubsystem(const EngineConfig::RenderConfig& Config, WindowSubsystem& Window)
		: RenderResolution{ Config.ResolutionX, Config.ResolutionY }
		, WindowSize{ Window.getSize() }
		, RenderWindow{ Window }
	{
		// Initialize main render targets at fixed render resolution
		VERIFY(WorldRenderTarget.resize(RenderResolution), "Failed to resize WorldRenderTarget");
		VERIFY(ScreenUIRenderTarget.resize(RenderResolution), "Failed to resize ScreenUIRenderTarget");
		VERIFY(WorldUIRenderTarget.resize(RenderResolution), "Failed to resize WorldUIRenderTarget");
		VERIFY(CompositeTarget.resize(RenderResolution), "Failed to resize CompositeTarget");
		VERIFY(CursorRenderTarget.resize(WindowSize), "Failed to resize CursorRenderTarget");

		// Set smoothing per config
		WorldRenderTarget.setSmooth(Config.WorldSmooth);
		ScreenUIRenderTarget.setSmooth(Config.ScreenUISmooth);
		WorldUIRenderTarget.setSmooth(Config.WorldUISmooth);
		CursorRenderTarget.setSmooth(Config.CursorSmooth);
		CompositeTarget.setSmooth(false);

		// Initialize post-process targets (always created, used if effects added)
		VERIFY(WorldPostProcessTarget.resize(RenderResolution), "Failed to resize WorldPostProcessTarget");
		VERIFY(ScreenUIPostProcessTarget.resize(RenderResolution), "Failed to resize ScreenUIPostProcessTarget");
		VERIFY(WorldUIPostProcessTarget.resize(RenderResolution), "Failed to resize WorldUIPostProcessTarget");
		VERIFY(CursorPostProcessTarget.resize(RenderResolution), "Failed to resize CursorPostProcessTarget");

		LOG("RenderSubsystem initialized: {}x{}", Config.ResolutionX, Config.ResolutionY);
	}

	RenderSubsystem::~RenderSubsystem() = default;

	// =========================================================================
	// Frame Management
	// =========================================================================

	void RenderSubsystem::BeginFrame()
	{
		// Resize cursor target if window size changed
		vec2u NewWindowSize = RenderWindow.getSize();
		if (CursorRenderTarget.getSize() != NewWindowSize)
		{
			WindowSize = NewWindowSize;
			(void)CursorRenderTarget.resize(WindowSize);
		}

		ClearRenderTargets();
	}

	void RenderSubsystem::EndFrame()
	{
		// Display all main targets
		WorldRenderTarget.display();
		ScreenUIRenderTarget.display();
		WorldUIRenderTarget.display();
		CursorRenderTarget.display();
	}

	void RenderSubsystem::ClearRenderTargets()
	{
		// World gets background color
		WorldRenderTarget.clear(color::Black);

		// UI layers transparent
		ScreenUIRenderTarget.clear(color::Transparent);
		WorldUIRenderTarget.clear(color::Transparent);
		CompositeTarget.clear(color::Transparent);
		CursorRenderTarget.clear(color::Transparent);

		// Post-process targets
		WorldPostProcessTarget.clear(color::Transparent);
		ScreenUIPostProcessTarget.clear(color::Transparent);
		WorldUIPostProcessTarget.clear(color::Transparent);
		CursorPostProcessTarget.clear(color::Transparent);
	}

	// =========================================================================
	// Drawing
	// =========================================================================

	void RenderSubsystem::Draw(const drawable& Object, ERenderLayer Layer)
	{
		// Default view space based on layer
		EViewSpace DefaultSpace = (Layer == ERenderLayer::World || Layer == ERenderLayer::WorldUI)
			? EViewSpace::World
			: EViewSpace::Screen;
		
		Draw(Object, Layer, DefaultSpace);
	}

	void RenderSubsystem::Draw(const drawable& Object, ERenderLayer Layer, EViewSpace ViewSpace)
	{
		renderTexture& Target = GetMainTargetForLayer(Layer);

		if (Layer == ERenderLayer::Cursor)
		{
			// Cursor always uses default view
			Target.setView(Target.getDefaultView());
		}
		else if (ViewSpace == EViewSpace::Screen)
		{
			// Screen space
			Target.setView(Target.getDefaultView());
		}
		else
		{
			// World space - use current world view
			Target.setView(CurrentWorldView);
		}

		Target.draw(Object);
	}

	// =========================================================================
	// Camera / View
	// =========================================================================

	void RenderSubsystem::SetWorldView(const std::optional<CameraView>& View)
	{
		view WorldView;

		if (View.has_value())
		{
			// Use provided camera view
			float AspectRatio = static_cast<float>(RenderResolution.x) / RenderResolution.y;
			vec2f ViewSize = View->GetViewSize(AspectRatio);

			WorldView.setCenter(View->Position);
			WorldView.setSize(ViewSize);
			WorldView.setRotation(sf::radians(View->Rotation));
		}
		else
		{
			// Default view centered at screen center with editor offset and zoom
			WorldView.setCenter((vec2f(RenderResolution) / 2.0f) + EditorCameraOffset);
			WorldView.setSize(vec2f(RenderResolution) / EditorZoom);
		}

		WorldView.setViewport(rectf({ 0.f, 0.f }, { 1.f, 1.f }));

		// Cache and apply to world targets
		CurrentWorldView = WorldView;
		WorldRenderTarget.setView(WorldView);
		WorldPostProcessTarget.setView(WorldView);
		WorldUIRenderTarget.setView(WorldView);
		WorldUIPostProcessTarget.setView(WorldView);
	}

	// =========================================================================
	// Editor Grid
	// =========================================================================

	void RenderSubsystem::DrawEditorGrid(float GridSpacing, int Subdivisions)
	{
		// Calculate visible area based on current view
		vec2f viewCenter = CurrentWorldView.getCenter();
		vec2f viewSize = CurrentWorldView.getSize();
		
		float left = viewCenter.x - viewSize.x * 0.5f;
		float right = viewCenter.x + viewSize.x * 0.5f;
		float top = viewCenter.y - viewSize.y * 0.5f;
		float bottom = viewCenter.y + viewSize.y * 0.5f;
		
		// Draw grid lines
		if (bGridEnabled)
		{
			// Round to nearest grid line
			float startX = std::floor(left / GridSpacing) * GridSpacing;
			float endX = std::ceil(right / GridSpacing) * GridSpacing;
			float startY = std::floor(top / GridSpacing) * GridSpacing;
			float endY = std::ceil(bottom / GridSpacing) * GridSpacing;
			
			sf::VertexArray lines(sf::PrimitiveType::Lines);
			
			// Vertical grid lines
			for (float x = startX; x <= endX; x += GridSpacing)
			{
				bool isAxis = (std::abs(x) < 0.001f);
				color lineColor = isAxis ? GridXAxisColor : GridLineColor;
				
				lines.append(sf::Vertex(vec2f(x, top), lineColor));
				lines.append(sf::Vertex(vec2f(x, bottom), lineColor));
			}
			
			// Horizontal grid lines  
			for (float y = startY; y <= endY; y += GridSpacing)
			{
				bool isAxis = (std::abs(y) < 0.001f);
				color lineColor = isAxis ? GridYAxisColor : GridLineColor;
				
				lines.append(sf::Vertex(vec2f(left, y), lineColor));
				lines.append(sf::Vertex(vec2f(right, y), lineColor));
			}
			
			// Draw to world target
			WorldRenderTarget.draw(lines);
		}
		
		// Draw origin point
		if (bGridOriginEnabled)
		{
			if (left <= 0 && right >= 0 && top <= 0 && bottom >= 0)
			{
				// Draw white circle at origin
				circle originCircle(8.0f / EditorZoom);
				originCircle.setFillColor(color::White);
				originCircle.setOrigin({ 8.0f / EditorZoom, 8.0f / EditorZoom });
				originCircle.setPosition({ 0, 0 });
				WorldRenderTarget.draw(originCircle);
				
				// Draw black outline
				originCircle.setFillColor(color::Transparent);
				originCircle.setOutlineColor(color::Black);
				originCircle.setOutlineThickness(2.0f / EditorZoom);
				WorldRenderTarget.draw(originCircle);
			}
		}
	}

	// =========================================================================
	// Layer Access
	// =========================================================================

	renderTexture& RenderSubsystem::GetMainTargetForLayer(ERenderLayer Layer)
	{
		switch (Layer)
		{
		case ERenderLayer::World:     return WorldRenderTarget;
		case ERenderLayer::ScreenUI:  return ScreenUIRenderTarget;
		case ERenderLayer::WorldUI:   return WorldUIRenderTarget;
		case ERenderLayer::Cursor:    return CursorRenderTarget;
		default:                      return WorldRenderTarget;
		}
	}

	renderTexture& RenderSubsystem::GetPostProcessTargetForLayer(ERenderLayer Layer)
	{
		switch (Layer)
		{
		case ERenderLayer::World:     return WorldPostProcessTarget;
		case ERenderLayer::ScreenUI:  return ScreenUIPostProcessTarget;
		case ERenderLayer::WorldUI:   return WorldUIPostProcessTarget;
		case ERenderLayer::Cursor:    return CursorPostProcessTarget;
		default:                      return WorldPostProcessTarget;
		}
	}

	vector<unique<IPostProcessEffect>>& RenderSubsystem::GetEffectsForLayer(ERenderLayer Layer)
	{
		switch (Layer)
		{
		case ERenderLayer::World:     return WorldPostProcessEffects;
		case ERenderLayer::ScreenUI:  return ScreenUIPostProcessEffects;
		case ERenderLayer::WorldUI:   return WorldUIPostProcessEffects;
		case ERenderLayer::Cursor:    return CursorPostProcessEffects;
		default:                      return WorldPostProcessEffects;
		}
	}

	// =========================================================================
	// Post-Processing
	// =========================================================================

	void RenderSubsystem::AddPostProcessEffect(ERenderLayer Layer, unique<IPostProcessEffect> Effect)
	{
		GetEffectsForLayer(Layer).push_back(std::move(Effect));
	}

	const texture& RenderSubsystem::GetLayerTexture(ERenderLayer Layer)
	{
		renderTexture& MainTarget = GetMainTargetForLayer(Layer);
		renderTexture& PostTarget = GetPostProcessTargetForLayer(Layer);
		vector<unique<IPostProcessEffect>>& Effects = GetEffectsForLayer(Layer);

		if (!Effects.empty())
		{
			return ProcessPostEffects(MainTarget, PostTarget, Effects).getTexture();
		}

		MainTarget.display();
		return MainTarget.getTexture();
	}

	renderTexture& RenderSubsystem::ProcessPostEffects(
		renderTexture& Input,
		renderTexture& Output,
		vector<unique<IPostProcessEffect>>& Effects)
	{
		if (Effects.empty())
		{
			Input.display();
			return Input;
		}

		Input.display();
		renderTexture* In = &Input;
		renderTexture* Out = &Output;

		for (auto& Effect : Effects)
		{
			Out->clear(color::Transparent);
			// Effect->Apply(In->getTexture(), *Out);  // TODO: Implement IPostProcessEffect
			Out->display();
			std::swap(In, Out);
		}

		return *In;
	}

	// =========================================================================
	// Compositing & Output
	// =========================================================================

	void RenderSubsystem::CompositeLayers()
	{
		// Get textures (with post-processing if available)
		const texture& WorldTex = GetLayerTexture(ERenderLayer::World);
		const texture& WorldUITex = GetLayerTexture(ERenderLayer::WorldUI);
		const texture& ScreenUITex = GetLayerTexture(ERenderLayer::ScreenUI);

		// Composite to final target
		CompositeTarget.clear(color::Transparent);
		CompositeTarget.setView(CompositeTarget.getDefaultView());

		// World layer
		sprite WorldSprite(WorldTex);
		CompositeTarget.draw(WorldSprite);

		// World UI (follows world camera, rendered on top)
		sprite WorldUISprite(WorldUITex);
		CompositeTarget.draw(WorldUISprite, sf::BlendAlpha);

		// Screen UI (fixed to screen)
		sprite ScreenUISprite(ScreenUITex);
		CompositeTarget.draw(ScreenUISprite, sf::BlendAlpha);

		CompositeTarget.display();
	}

	sprite RenderSubsystem::FinishComposite()
	{
		CompositeLayers();

		// Calculate letterbox/pillarbox to fit render resolution into window
		// Get FRESH window size (may have changed due to resize/fullscreen)
		vec2u CurrentWindowSize = RenderWindow.getSize();
		float ScaleX = static_cast<float>(CurrentWindowSize.x) / RenderResolution.x;
		float ScaleY = static_cast<float>(CurrentWindowSize.y) / RenderResolution.y;

		// Maintain aspect ratio (fit inside window)
		float Scale = std::min(ScaleX, ScaleY);

		// Center the sprite
		float PosX = (CurrentWindowSize.x - (RenderResolution.x * Scale)) / 2.f;
		float PosY = (CurrentWindowSize.y - (RenderResolution.y * Scale)) / 2.f;

		// Build final sprite
		sprite FinalSprite(CompositeTarget.getTexture());
		FinalSprite.setScale({ Scale, Scale });
		FinalSprite.setPosition({ PosX, PosY });

		// Cache the letterbox view for coordinate mapping
		UpdateLetterboxView(Scale, PosX, PosY);

		return FinalSprite;
	}

	void RenderSubsystem::UpdateLetterboxView(float Scale, float PosX, float PosY)
	{
		LetterboxView.setSize(vec2f(RenderResolution));
		LetterboxView.setCenter(vec2f(RenderResolution) / 2.0f);
		
		// Calculate viewport in normalized coordinates (0-1 range)
		vec2u CurrentWindowSize = RenderWindow.getSize();
		LetterboxView.setViewport({
			{ PosX / CurrentWindowSize.x, PosY / CurrentWindowSize.y },
			{ (RenderResolution.x * Scale) / CurrentWindowSize.x, (RenderResolution.y * Scale) / CurrentWindowSize.y }
		});
	}

	void RenderSubsystem::PresentCursor()
	{
		CursorRenderTarget.display();

		// Get current window size for 1:1 cursor rendering
		vec2u CurrentWindowSize = RenderWindow.getSize();

		// Resize cursor target if window size changed
		if (CursorRenderTarget.getSize() != CurrentWindowSize)
		{
			(void)CursorRenderTarget.resize(CurrentWindowSize);
		}

		// Draw cursor 1:1 to window (no scaling)
		view WindowView(sf::FloatRect({ 0.f, 0.f }, vec2f(CurrentWindowSize)));
		RenderWindow.setView(WindowView);

		sprite CursorSprite(CursorRenderTarget.getTexture());
		RenderWindow.draw(CursorSprite, sf::BlendAlpha);
	}

	const texture& RenderSubsystem::GetWorldTexture() const
	{
		return WorldRenderTarget.getTexture();
	}

	vec2f RenderSubsystem::MapPixelToCoords(vec2i PixelPos) const
	{
		return RenderWindow.mapPixelToCoords(PixelPos, LetterboxView);
	}
}
