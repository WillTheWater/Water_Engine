#include "Framework/MathUtility.h"

namespace we
{
    sf::Vector2f RotationToVector(const sf::Angle Rotation)
    {
        const float radians = Rotation.asRadians();
        return sf::Vector2f(std::cos(radians), std::sin(radians));
    }

}
