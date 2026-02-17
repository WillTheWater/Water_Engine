// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "UI/Widget/Button.h"
#include "Utility/Log.h"
#include "EngineConfig.h"
#include "Subsystem/ResourceSubsystem.h"

namespace we
{
	Button::Button(
		const string& InLabel,
		const vec2f& InSize,
		color FillColor,
		color InOutlineColor,
		float InOutlineThickness)
		: Label(InLabel)
		, NormalColor(FillColor)
		, OutlineColor(InOutlineColor)
		, OutlineThickness(InOutlineThickness)
		, bUseTexture(false)
		, bUseColorTint(false)
		, bHasTextureRects(false)
	{
		SetSize(InSize);
		SetFocusable(true);

		BackgroundRect.setSize(InSize);
		BackgroundRect.setFillColor(NormalColor);
		BackgroundRect.setOutlineColor(OutlineColor);
		BackgroundRect.setOutlineThickness(OutlineThickness);

		TextFont = LoadAsset().LoadFontSync(EC.DefaultFont);
		if (TextFont && !Label.empty())
		{
			LabelText.emplace(*TextFont, Label, 24);
			LabelText->setFillColor(color::Black);
		}

		OnFocusGained.Bind(this, &Button::OnButtonFocusGained);
		OnFocusLost.Bind(this, &Button::OnButtonFocusLost);
	}

	Button::Button(
		const string& InLabel,
		const string& TexturePath,
		const vec2f& InSize,
		bool bInUseColorTint)
		: Label(InLabel)
		, bUseTexture(true)
		, bUseColorTint(bInUseColorTint)
		, bHasTextureRects(false)
	{
		SetFocusable(true);

		LoadTexture(ButtonTexture, TexturePath);
		if (ButtonTexture)
		{
			vec2f TexSize = vec2f(ButtonTexture->getSize());

			if (InSize.x == 0.f || InSize.y == 0.f)
			{
				SetSize({ TexSize.x, TexSize.y / 3.f });
			}
			else
			{
				SetSize(InSize);
			}

			NormalTextureRect = { {0, 0}, {static_cast<int>(TexSize.x), static_cast<int>(TexSize.y / 3)} };
			HoverTextureRect = { {0, static_cast<int>(TexSize.y / 3)}, {static_cast<int>(TexSize.x), static_cast<int>(TexSize.y / 3)} };
			PressedTextureRect = { {0, static_cast<int>(TexSize.y * 2 / 3)}, {static_cast<int>(TexSize.x), static_cast<int>(TexSize.y / 3)} };
			bHasTextureRects = true;

			BackgroundSprite.emplace(*ButtonTexture, NormalTextureRect);
			BackgroundSprite->setColor(color::White);
		}
		else
		{
			SetSize({ 150.f, 50.f });
		}

		TextFont = LoadAsset().LoadFontSync(EC.DefaultFont);
		if (TextFont && !Label.empty())
		{
			LabelText.emplace(*TextFont, Label, 24);
			LabelText->setFillColor(color::Black);
		}

		OnFocusGained.Bind(this, &Button::OnButtonFocusGained);
		OnFocusLost.Bind(this, &Button::OnButtonFocusLost);
	}

	void Button::CollectRenderDepths(vector<RenderDepth>& OutDepths) const
	{
		if (!IsVisible()) return;

		vec2f Pos = GetWorldPosition();
		vec2f Scl = GetWorldScale();

		if (bUseTexture && BackgroundSprite)
		{
			BackgroundSprite->setPosition(Pos);
			BackgroundSprite->setScale(Scl);
			OutDepths.push_back({ &*BackgroundSprite, GetRenderDepth() });
		}
		else
		{
			BackgroundRect.setPosition(Pos);
			BackgroundRect.setScale(Scl);
			OutDepths.push_back({ &BackgroundRect, GetRenderDepth() });
		}

		if (LabelText)
		{
			vec2f Size = GetSize().componentWiseMul(Scl);
			vec2f Center = Pos + Size.componentWiseDiv({ 2.f, 2.f });

			auto Bounds = LabelText->getLocalBounds();
			LabelText->setOrigin({ 
				Bounds.position.x + Bounds.size.x / 2.f,
				Bounds.position.y + Bounds.size.y / 2.f 
			});
			LabelText->setPosition(Center);

			OutDepths.push_back({ &*LabelText, GetRenderDepth() + 1.f });
		}
	}

	void Button::SetNormalColor(color InColor)
	{
		NormalColor = InColor;
		UpdateVisualState();
	}

	void Button::SetHoverColor(color InColor)
	{
		HoverColor = InColor;
		UpdateVisualState();
	}

	void Button::SetPressedColor(color InColor)
	{
		PressedColor = InColor;
		UpdateVisualState();
	}

	void Button::SetNormalTextureRect(const recti& Rect)
	{
		NormalTextureRect = Rect;
		bHasTextureRects = true;
		UpdateVisualState();
	}

	void Button::SetHoverTextureRect(const recti& Rect)
	{
		HoverTextureRect = Rect;
		bHasTextureRects = true;
		UpdateVisualState();
	}

	void Button::SetPressedTextureRect(const recti& Rect)
	{
		PressedTextureRect = Rect;
		bHasTextureRects = true;
		UpdateVisualState();
	}

	void Button::UpdateVisualState()
	{
		if (bUseTexture && BackgroundSprite)
		{
			if (bHasTextureRects && !bUseColorTint)
			{
				if (bPressed)
				{
					BackgroundSprite->setTextureRect(PressedTextureRect);
				}
				else if (bHovered)
				{
					BackgroundSprite->setTextureRect(HoverTextureRect);
				}
				else
				{
					BackgroundSprite->setTextureRect(NormalTextureRect);
				}
				BackgroundSprite->setColor(color::White);
			}
			else
			{
				color TargetColor = bPressed ? PressedColor : (bHovered ? HoverColor : NormalColor);
				BackgroundSprite->setColor(TargetColor);
			}
		}
		else
		{
			color TargetColor = bPressed ? PressedColor : (bHovered ? HoverColor : NormalColor);
			BackgroundRect.setFillColor(TargetColor);
		}
	}

	void Button::SetPressed(bool bInPressed)
	{
		bPressed = bInPressed;
		UpdateVisualState();
	}

	void Button::OnButtonFocusGained()
	{
		bHovered = true;
		UpdateVisualState();
	}

	void Button::OnButtonFocusLost()
	{
		bHovered = false;
		bPressed = false;
		UpdateVisualState();
	}

	void Button::LoadTexture(shared<texture>& OutTexture, const string& Path)
	{
		if (!Path.empty())
		{
			OutTexture = LoadAsset().LoadTextureSync(Path);
		}
	}
}
