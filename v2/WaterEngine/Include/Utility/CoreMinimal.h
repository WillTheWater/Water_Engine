// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include <memory>
#include <optional>
#include <variant>
#include <functional>
#include <algorithm>
#include <numbers>
#include <map>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <fstream>
#include <sfml/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/Audio.hpp>
#include <nlohmann/json.hpp>

namespace we
{
	using string = std::string;
	using stringView = std::string_view;
	using filestream = std::fstream;
	using outstream = std::ofstream;
	using json = nlohmann::json;

	using uint8 = unsigned char;
	using uint32 = uint32_t;
	using uint = std::uint32_t;
	
	template<typename T>
	using vec2 = sf::Vector2<T>;

	using vec2f = sf::Vector2f;
	using vec2i = sf::Vector2i;
	using vec2u = sf::Vector2u;
	using vec3f = sf::Vector3f;

	using angle = sf::Angle;

	using sprite = sf::Sprite;
	using texture = sf::Texture;
	using image = sf::Image;
	using font = sf::Font;
	using soundBuffer = sf::SoundBuffer;
	using renderTexture = sf::RenderTexture;
	using circle = sf::CircleShape;
	using shape = sf::Shape;
	using rectangle = sf::RectangleShape;
	using drawable = sf::Drawable;
	using view = sf::View;
	using rectf = sf::FloatRect;
	using color = sf::Color;

	template<typename T>
	using unique = std::unique_ptr<T>;

	template<typename T, typename... Args>
	std::unique_ptr<T> make_unique(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using shared = std::shared_ptr<T>;

	template<typename T, typename... Args>
	std::shared_ptr<T> make_shared(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using weak = std::weak_ptr<T>;

	template<typename T>
	std::weak_ptr<T> make_weak(const std::shared_ptr<T>& shared)
	{
		return std::weak_ptr<T>(shared);
	}

	template<typename T>
	using optional = std::optional<T>;

	template<typename T, std::size_t N>
	using arr = std::array<T, N>;

	template<typename T>
	using list = std::vector<T>;

	template<typename T>
	using set = std::unordered_set<T>;

	template<typename Key, typename Type, typename P = std::less<Key>>
	using map = std::map<Key, Type, P>;

	template<typename Key, typename Type, typename Hasher = std::hash<Key>>
	using dictionary = std::unordered_map<Key, Type, Hasher>;

	template<typename Key, typename Type, typename Hasher = std::hash<Key>>
	using library = std::unordered_multimap<Key, Type, Hasher>;
}