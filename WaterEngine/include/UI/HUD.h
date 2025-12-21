#pragma once
#include "Framework/Object.h"
#include "SFML/Graphics.hpp"

namespace we
{
	class Renderer;

    class HUD : public Object
    {
    public:
        virtual void Render(Renderer& GameRenderer) = 0;
        virtual bool HandleEvent(const optional<sf::Event> Event);

        void NativeInitialize(Renderer& GameRenderer);
        bool IsInitialized() const { return bIsInitialized; }

    protected:
        HUD();

    private:
        virtual void Initialize(Renderer& GameRenderer);
        bool bIsInitialized;
    };
}