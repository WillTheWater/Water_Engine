#include "Framework/PropActor.h"

namespace we
{
    Prop::Prop(World* OwningWorld, const string& TexturePath)
        : Actor{ OwningWorld, TexturePath }
        , Velocity{}
        , bIsTiled{ false }
        , bIsMoving{ false }
        , TextureSize{ 0, 0 }
        , TextureShift{ 0.f, 0.f }
    {
    }

    void Prop::BeginPlay()
    {
    }

    void Prop::Tick(float DeltaTime)
    {
        if (bIsMoving)
        {
            Move(DeltaTime);
        }
    }

    void Prop::SetTextureTiled(bool Tile)
    {
        Actor::SetTextureTiled(Tile);
        bIsTiled = Tile;

        if (Tile)
        {
            TextureSize = GetTextureSize();

            if (TextureSize.x > 0 && TextureSize.y > 0)
            {
                sf::Vector2u ScreenSize = GetWindowSize();

                SetSpriteFrame(ScreenSize.x, ScreenSize.y);

                SetActorLocation({ 0.f, 0.f });
            }
        }
    }

    void Prop::Move(float DeltaTime)
    {
        if (!bIsTiled)
        {
            AddActorLocationOffset(Velocity * DeltaTime);
            return;
        }

        TextureShift += Velocity * DeltaTime;

        if (TextureSize.x > 0) TextureShift.x = std::fmod(TextureShift.x, (float)TextureSize.x);
        if (TextureSize.y > 0) TextureShift.y = std::fmod(TextureShift.y, (float)TextureSize.y);

        int NewLeft = static_cast<int>(TextureShift.x);
        int NewTop = static_cast<int>(TextureShift.y);

        sf::Vector2u ScreenSize = GetWindowSize();
        sf::IntRect Rect;

        Rect.position.x = NewLeft;
        Rect.position.y = NewTop;
        Rect.size.x = static_cast<int>(ScreenSize.x);
        Rect.size.y = static_cast<int>(ScreenSize.y);
        SetTextureRect(Rect);
    }

    void Prop::SetVelocity(const sf::Vector2f& NewVelocity)
    {
        Velocity = -NewVelocity;
    }

    void Prop::SetIsMoving(bool ShouldMove)
    {
        bIsMoving = ShouldMove;
    }
}