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

        virtual void SetTextureTiled(bool Tile) override;

    private:
        void Move(float DeltaTime);
        sf::Vector2f Velocity;
        bool bIsTiled;
        bool bIsMoving;
        sf::Vector2u TextureSize;
    };
}