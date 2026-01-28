#pragma once
#include <stdio.h>
#include <memory>
#include <optional>
#include <functional>
#include <map>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>

namespace we
{
	using string = std::string;
	using uint8 = unsigned char;

	template<typename T>
	inline T clamp(const T& value, const T& min, const T& max)
	{
		return std::clamp(value, min, max);
	}

	template<typename T>
	using unique = std::unique_ptr<T>;

	template<typename T>
	using shared = std::shared_ptr<T>;

	template<typename T>
	using weak = std::weak_ptr<T>;

	template<typename T>
	using optional = std::optional<T>;

	template<typename T>
	using List = std::vector<T>;
	
	template<typename T>
	using Set = std::unordered_set<T>;

	template<typename Key, typename Type, typename P = std::less<Key>>
	using Map = std::map<Key, Type, P>;

	template<typename Key, typename Type, typename Hasher = std::hash<Key>>
	using Dictionary = std::unordered_map<Key, Type, Hasher>;

#define LOG(Message, ...) printf(Message "\n", ##__VA_ARGS__);
}