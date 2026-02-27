// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

// =============================================================================
// Standard Library
// =============================================================================
#include <algorithm>
#include <array>
#include <cstdint>
#include <functional>
#include <fstream>
#include <map>
#include <memory>
#include <numbers>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

// =============================================================================
// Third Party - SFML
// =============================================================================
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>

// =============================================================================
// Core Types
// =============================================================================
namespace we
{
	// =========================================================================
	// Basic Types
	// =========================================================================
	using string      = std::string;
	using stringView  = std::string_view;
	using filestream  = std::fstream;
	using outstream   = std::ofstream;

	using uint8  = unsigned char;
	using uint   = std::uint32_t;
	using int64  = std::int64_t;
	using ulong  = std::size_t;

	// =========================================================================
	// Math Types
	// =========================================================================
	template<typename T>
	using vec2 = sf::Vector2<T>;

	using vec2f  = sf::Vector2f;
	using vec2i  = sf::Vector2i;
	using vec2u  = sf::Vector2u;
	using vec3f  = sf::Vector3f;
	using rectf  = sf::FloatRect;
	using recti  = sf::IntRect;
	using angle  = sf::Angle;
	using clock  = sf::Clock;
	using time   = sf::Time;

	// =========================================================================
	// SFML Graphics Types
	// =========================================================================
	using drawable      = sf::Drawable;
	using sprite        = sf::Sprite;
	using texture       = sf::Texture;
	using image         = sf::Image;
	using rectangle     = sf::RectangleShape;
	using circle        = sf::CircleShape;
	using shape         = sf::Shape;
	using text          = sf::Text;
	using font          = sf::Font;
	using color         = sf::Color;
	using view          = sf::View;
	using renderTarget  = sf::RenderTarget;
	using renderTexture = sf::RenderTexture;
	using shader        = sf::Shader;

	// =========================================================================
	// SFML Audio Types
	// =========================================================================
	using soundBuffer   = sf::SoundBuffer;
	using sound         = sf::Sound;
	using music         = sf::Music;

	// =========================================================================
	// Math Constants
	// =========================================================================
	constexpr float PI   = std::numbers::pi_v<float>;
	constexpr float PI_2 = PI / 2.0f;
	constexpr float PI_4 = PI / 4.0f;
	constexpr float TAU  = 2.0f * PI;

	// =========================================================================
	// Memory
	// =========================================================================
	template<typename T>
	using unique = std::unique_ptr<T>;

	template<typename T, typename... Args>
	unique<T> make_unique(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using shared = std::shared_ptr<T>;

	template<typename T, typename... Args>
	shared<T> make_shared(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using weak = std::weak_ptr<T>;

	template<typename T>
	weak<T> make_weak(const shared<T>& shared)
	{
		return weak<T>(shared);
	}

	// =========================================================================
	// Containers
	// =========================================================================
	template<typename T>
	using optional = std::optional<T>;

	template<typename T, std::size_t N>
	using array = std::array<T, N>;

	template<typename T>
	using vector = std::vector<T>;

	template<typename T>
	using set = std::unordered_set<T>;

	template<typename Key, typename Type, typename P = std::less<Key>>
	using map = std::map<Key, Type, P>;

	template<typename Key, typename Type, typename Hasher = std::hash<Key>>
	using dictionary = std::unordered_map<Key, Type, Hasher>;

	template<typename Key, typename Type, typename Hasher = std::hash<Key>>
	using library = std::unordered_multimap<Key, Type, Hasher>;
}
