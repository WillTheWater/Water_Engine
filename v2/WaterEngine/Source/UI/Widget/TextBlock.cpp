//// =============================================================================
//// Water Engine v2.0.0
//// Copyright(C) 2026 Will The Water
//// =============================================================================
//
//#include "UI/Widget/TextBlock.h"
//#include "Subsystem/WindowSubsystem.h"
//#include "Subsystem/ResourceSubsystem.h"
//#include "EngineConfig.h"
//#include "Utility/Log.h"
//
//namespace we
//{
//    TextBlock::TextBlock(EngineSubsystem& InSubsystem, const string& InText, float InWidth, float LineSpace)
//        : Widget(InSubsystem)
//        , RawText(InText)
//        , FontResource(Asset().LoadFont(EC.DefaultTitleFont))
//        , TextComponent(*FontResource, InText, CharSize)
//        , LineSpacing{LineSpace}
//    {
//        TextComponent.setFillColor(color::Black);
//        Size.x = InWidth;
//        UpdateWrappedText();
//    }
//
//    void TextBlock::SetText(const string& InText)
//    {
//        if (RawText != InText)
//        {
//            RawText = InText;
//            bNeedsWrapUpdate = true;
//        }
//    }
//
//    const string& TextBlock::GetText() const
//    {
//        return RawText;
//    }
//
//    void TextBlock::SetFont(shared<font> InFont)
//    {
//        FontResource = InFont;
//        if (FontResource)
//            TextComponent.setFont(*FontResource);
//
//        bNeedsWrapUpdate = true;
//    }
//
//    void TextBlock::SetCharacterSize(uint Size)
//    {
//        CharSize = Size;
//        TextComponent.setCharacterSize(Size);
//        bNeedsWrapUpdate = true;
//    }
//
//    void TextBlock::SetColor(const color& InColor)
//    {
//        TextComponent.setFillColor(InColor);
//    }
//
//    void TextBlock::Render(GameWindow& Window)
//    {
//        if (!IsVisible() || !FontResource)
//        {
//            Widget::RenderChildren(Window);
//            return;
//        }
//
//        if (bNeedsWrapUpdate)
//            UpdateWrappedText();
//
//        sf::FloatRect Bounds = TextComponent.getLocalBounds();
//        vec2f WorldPos = GetWorldPosition();
//
//        TextComponent.setPosition(WorldPos - vec2f{ Bounds.position.x, Bounds.position.y });
//        TextComponent.setScale(GetWorldScale());
//
//        Window.draw(TextComponent);
//        Widget::RenderChildren(Window);
//    }
//
//    void TextBlock::UpdateWrappedText()
//    {
//        if (!FontResource) return;
//
//        TextComponent.setLineSpacing(LineSpacing);
//        TextComponent.setCharacterSize(CharSize);
//
//        float MaxWidth = Size.x;
//        string WrappedString;
//        string CurrentLine;
//        string CurrentWord;
//        int LineCount = 1;
//
//        auto GetWordWidth = [&](const string& Str) -> float {
//            float Width = 0.f;
//            for (char c : Str) {
//                const auto& Glyph = FontResource->getGlyph(static_cast<std::uint32_t>(c), CharSize, false);
//                Width += Glyph.advance;
//            }
//            return Width;
//            };
//
//        float SpaceWidth = FontResource->getGlyph(' ', CharSize, false).advance;
//
//        auto ProcessText = RawText + " ";
//        for (char c : ProcessText)
//        {
//            if (c == '\n')
//            {
//                WrappedString += CurrentLine + CurrentWord + "\n";
//                CurrentLine.clear();
//                CurrentWord.clear();
//                LineCount++;
//            }
//            else if (c == ' ' || c == '\t')
//            {
//                float LineW = GetWordWidth(CurrentLine);
//                float WordW = GetWordWidth(CurrentWord);
//
//                if (LineW + WordW > MaxWidth && !CurrentLine.empty())
//                {
//                    WrappedString += CurrentLine + "\n";
//                    CurrentLine = CurrentWord + " ";
//                    LineCount++;
//                }
//                else
//                {
//                    CurrentLine += CurrentWord + " ";
//                }
//                CurrentWord.clear();
//            }
//            else
//            {
//                CurrentWord += c;
//            }
//        }
//
//        WrappedString += CurrentLine;
//        TextComponent.setString(WrappedString);
//
//        float LogicalLineHeight = FontResource->getLineSpacing(CharSize) * LineSpacing;
//        float TotalHeight = LogicalLineHeight * static_cast<float>(LineCount);
//
//        SetSize({ MaxWidth, TotalHeight });
//
//        bNeedsWrapUpdate = false;
//    }
//}