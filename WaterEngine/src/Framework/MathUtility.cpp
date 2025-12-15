#include "Framework/MathUtility.h"

namespace we
{
    sf::Vector2f RotationToVector(const sf::Angle Rotation)
    {
        const float radians = Rotation.asRadians();
        return sf::Vector2f(std::cos(radians), std::sin(radians));
    }

    sf::Angle VectorToRotation(const sf::Vector2f Vector)
    {
        return sf::radians(std::atan2(Vector.y, Vector.x));
    }
}
