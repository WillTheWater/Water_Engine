#include "Framework/PropActor.h"

namespace we
{
    Prop::Prop(World* OwningWorld, const string& TexturePath)
        : Actor{ OwningWorld, TexturePath }
        , Velocity{}
        , bIsTiled{ false }
        , bIsMoving{ false }
        , TextureSize{ 0, 0 }
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
            
            sf::Vector2u ScreenSize = GetWindowSize();
            int FrameWidth = ScreenSize.x + TextureSize.x;
            int FrameHeight = ScreenSize.y + TextureSize.y;

            SetSpriteFrame(FrameWidth, FrameHeight);
        }
    }

    void Prop::Move(float DeltaTime)
    {
        if (!bIsTiled)
        {
            AddActorLocationOffset(Velocity * DeltaTime);
            return;
        }

        AddActorLocationOffset(Velocity * DeltaTime);
        auto ActorLocation = GetActorLocation();
        if (TextureSize.y > 0)
        {
            if (Velocity.y > 0 && ActorLocation.y > TextureSize.y)
            {
                ActorLocation.y -= TextureSize.y;
            }
            else if (Velocity.y < 0 && ActorLocation.y < -TextureSize.y)
            {
                ActorLocation.y += TextureSize.y;
            }
        }
    }

    void Prop::SetVelocity(const sf::Vector2f& NewVelocity)
    {
        Velocity = NewVelocity;
    }

    void Prop::SetIsMoving(bool ShouldMove)
    {
        bIsMoving = ShouldMove;
    }
}