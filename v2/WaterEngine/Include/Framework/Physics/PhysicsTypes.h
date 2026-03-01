// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	// =============================================================================
	// PhysicsType - Simulation behavior (matches Box2D body types)
	// Moved from Actor.h to share with PhysicsProxy
	// =============================================================================
	enum class PhysicsType { None, Static, Kinematic, Dynamic };

	// =============================================================================
	// CollisionChannel - Collision layers/filters
	// =============================================================================
	enum class CollisionChannel : uint
	{
		None           = 0,
		WorldStatic    = 1 << 0,   // 1 - Static geometry, walls, floors
		WorldDynamic   = 1 << 1,   // 2 - Moving platforms, physics objects
		Pawn           = 1 << 2,   // 4 - Player, NPCs, controllable characters
		PhysicsBody    = 1 << 3,   // 8 - Ragdolls, simulated objects
		Vehicle        = 1 << 4,   // 16 - Cars, boats, etc.
		Destructible   = 1 << 5,   // 32 - Breakable objects
		Projectile     = 1 << 6,   // 64 - Bullets, arrows, spells
		Item           = 1 << 7,   // 128 - Pickups, loot
		Sensor         = 1 << 8,   // 256 - AI detection zones
		Hitbox         = 1 << 9,   // 512 - Attack hitboxes
		Hurtbox        = 1 << 10,  // 1024 - Damage receiving areas
		Trigger        = 1 << 11,  // 2048 - General trigger volumes
		UI             = 1 << 12,  // 4096 - UI collision (cursor, etc.)
		
		// Game-specific channels start here (bit 16+)
		GameTraceChannel1 = 1 << 16,
		GameTraceChannel2 = 1 << 17,
		GameTraceChannel3 = 1 << 18,
		GameTraceChannel4 = 1 << 19,
		GameTraceChannel5 = 1 << 20,
		GameTraceChannel6 = 1 << 21,
	};

	// =============================================================================
	// Common collision mask presets
	// =============================================================================
	namespace CollisionPresets
	{
		// What each channel collides with by default
		constexpr uint WorldStatic_Mask    = static_cast<uint>(CollisionChannel::WorldStatic) 
		                                      | static_cast<uint>(CollisionChannel::WorldDynamic)
		                                      | static_cast<uint>(CollisionChannel::Pawn)
		                                      | static_cast<uint>(CollisionChannel::PhysicsBody)
		                                      | static_cast<uint>(CollisionChannel::Vehicle);
		
		constexpr uint Pawn_Mask           = static_cast<uint>(CollisionChannel::WorldStatic)
		                                      | static_cast<uint>(CollisionChannel::WorldDynamic)
		                                      | static_cast<uint>(CollisionChannel::Pawn)
		                                      | static_cast<uint>(CollisionChannel::PhysicsBody)
		                                      | static_cast<uint>(CollisionChannel::Vehicle)
		                                      | static_cast<uint>(CollisionChannel::Item);
		
		constexpr uint Projectile_Mask     = static_cast<uint>(CollisionChannel::WorldStatic)
		                                      | static_cast<uint>(CollisionChannel::WorldDynamic)
		                                      | static_cast<uint>(CollisionChannel::Pawn)
		                                      | static_cast<uint>(CollisionChannel::Vehicle)
		                                      | static_cast<uint>(CollisionChannel::Destructible)
		                                      | static_cast<uint>(CollisionChannel::Hurtbox);
		
		constexpr uint Sensor_Mask         = static_cast<uint>(CollisionChannel::Pawn)
		                                      | static_cast<uint>(CollisionChannel::Item);
		
		constexpr uint Hitbox_Mask         = static_cast<uint>(CollisionChannel::Hurtbox);
		
		constexpr uint Hurtbox_Mask        = static_cast<uint>(CollisionChannel::Hitbox)
		                                      | static_cast<uint>(CollisionChannel::Projectile);
		
		constexpr uint Item_Mask           = static_cast<uint>(CollisionChannel::WorldStatic)
		                                      | static_cast<uint>(CollisionChannel::Pawn);
	}

	// =============================================================================
	// ShapeDefinition - Shape creation parameters
	// =============================================================================
	struct ShapeDefinition
	{
		enum class Type : uint8
		{
			Box,
			Circle,
			Polygon
		};

		Type ShapeType = Type::Box;
		vec2f LocalOffset = { 0.0f, 0.0f };
		float LocalRotation = 0.0f;

		// Box
		vec2f HalfSize = { 0.0f, 0.0f };

		// Circle
		float Radius = 0.0f;

		// Polygon (max 8 vertices for Box2D)
		static constexpr uint MaxPolygonVertices = 8;
		vec2f Vertices[MaxPolygonVertices];
		uint VertexCount = 0;

		// Factory methods
		static ShapeDefinition MakeBox(vec2f halfSize, vec2f offset = { 0.0f, 0.0f })
		{
			ShapeDefinition def;
			def.ShapeType = Type::Box;
			def.HalfSize = halfSize;
			def.LocalOffset = offset;
			return def;
		}

		static ShapeDefinition MakeCircle(float radius, vec2f offset = { 0.0f, 0.0f })
		{
			ShapeDefinition def;
			def.ShapeType = Type::Circle;
			def.Radius = radius;
			def.LocalOffset = offset;
			return def;
		}
	};
}
