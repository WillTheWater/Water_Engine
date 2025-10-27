#pragma once
#include <stdio.h>
#include <memory>
#include <map>
#include <unordered_map>

namespace we
{
	template<typename T>
	using unique = std::unique_ptr<T>;

	template<typename T>
	using shared = std::shared_ptr<T>;
	
	template<typename T>
	using weak = std::weak_ptr<T>;

	template<typename T>
	using List = std::vector<T>;

	template<typename Key, typename Type, typename P = std::less<Key>>
	using Map = std::map<Key, Type, P>;
	
	template<typename Key, typename Type, typename Hasher = std::hash<Key>>
	using Dictionary = std::unordered_map<Key, Type, Hasher>;

	#define LOG(Message, ...) printf(Message "\n", ##__VA_ARGS__);
}