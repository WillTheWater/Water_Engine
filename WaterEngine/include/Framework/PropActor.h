#pragma once
#include "Framework/Actor.h"

namespace we
{
    class Prop : public Actor
    {
    public:
        Prop(World* OwningWorld, const string& TexturePath);

        virtual void BeginPlay() override;
        virtual void Tick(float DeltaTime) override;

        void SetIsMoving(bool ShouldMove);
        void SetVelocity(const sf::Vector2f& NewVelocity);

        // Override to setup Texture Rect logic
        void SetTextureTiled(bool Tile) override;

    private:
        void Move(float DeltaTime);
        sf::Vector2f Velocity;
        bool bIsTiled;
        bool bIsMoving;
        sf::Vector2u TextureSize;

        // This tracks how much we have scrolled in texture coordinates
        sf::Vector2f TextureShift;
    };
}