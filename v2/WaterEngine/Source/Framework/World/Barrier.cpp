// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#include "Framework/World/Barrier.h"
#include "Framework/World/World.h"
#include "Subsystem/PhysicsSubsystem.h"
#include "box2d/b2_body.h"
#include "box2d/b2_polygon_shape.h"
#include "box2d/b2_fixture.h"
#include "Utility/Log.h"
#include <cmath>

namespace we
{
	Barrier::Barrier(World& InWorld)
		: Actor(InWorld)
	{
	}

	Barrier::~Barrier() = default;

	void Barrier::SetPoints(const vector<vec2f>& InPoints)
	{
		Points = InPoints;
	}

	void Barrier::SetClosed(bool bInClosed)
	{
		bClosed = bInClosed;
	}

	void Barrier::SetThickness(float ThicknessPixels)
	{
		Thickness = ThicknessPixels;
	}

	void Barrier::BeginPlay()
	{
		Actor::BeginPlay();

		if (Points.size() < 2)
		{
			ERROR("[Barrier] Need at least 2 points");
			return;
		}

		CreateBody();
	}

	void Barrier::CreateBody()
	{
		auto& Physics = GetWorld().GetPhysics();

		b2BodyDef BodyDef;
		BodyDef.type = b2_staticBody;
		BodyDef.position = b2Vec2(0.0f, 0.0f);
		BodyDef.angle = 0.0f;

		Body = Physics.CreateBody(BodyDef);
		if (!Body)
		{
			ERROR("[Barrier] Failed to create body");
			return;
		}

		Body->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

		CreateSegments();

		LOG("[Barrier] Body created with {} segments", bClosed ? Points.size() : Points.size() - 1);
	}

	void Barrier::CreateSegments()
	{
		if (!Body || Points.size() < 2)
			return;

		auto& Physics = GetWorld().GetPhysics();

		size_t SegmentCount = bClosed ? Points.size() : Points.size() - 1;

		for (size_t i = 0; i < SegmentCount; ++i)
		{
			vec2f A = Points[i];
			vec2f B = Points[(i + 1) % Points.size()];

			// Calculate segment properties
			vec2f Midpoint = (A + B) / 2.0f;
			vec2f Delta = B - A;
			float Length = std::sqrt(Delta.x * Delta.x + Delta.y * Delta.y);
			float Angle = std::atan2(Delta.y, Delta.x);

			// Create oriented box for this segment
			b2PolygonShape BoxShape;
			BoxShape.SetAsBox(
				Physics.PixelsToMeters(Length / 2.0f),
				Physics.PixelsToMeters(Thickness / 2.0f),
				b2Vec2(Physics.PixelsToMeters(Midpoint.x), Physics.PixelsToMeters(Midpoint.y)),
				Angle
			);

			b2FixtureDef FixtureDef;
			FixtureDef.shape = &BoxShape;
			FixtureDef.density = 0.0f;
			FixtureDef.friction = 0.3f;
			FixtureDef.restitution = 0.0f;
			FixtureDef.isSensor = false;

			Body->CreateFixture(&FixtureDef);
		}
	}

	void Barrier::EndPlay()
	{
		DestroyBody();
		Actor::EndPlay();
	}

	void Barrier::DestroyBody()
	{
		if (!Body)
			return;

		LOG("[Barrier] Destroying body");

		GetWorld().GetPhysics().MarkForDestruction(Body);
		Body = nullptr;
	}

	void Barrier::GetDrawables(vector<const drawable*>& OutDrawables) const
	{
		// No sprite - barrier is invisible without debug
		(void)OutDrawables;

		if (!Body || Points.size() < 2)
			return;

		// Lazy initialize debug shapes
		if (!bDebugInitialized)
		{
			size_t SegmentCount = bClosed ? Points.size() : Points.size() - 1;
			DebugRects.reserve(SegmentCount);

			for (size_t i = 0; i < SegmentCount; ++i)
			{
				vec2f A = Points[i];
				vec2f B = Points[(i + 1) % Points.size()];

				vec2f Delta = B - A;
				float Length = std::sqrt(Delta.x * Delta.x + Delta.y * Delta.y);

				rectangle Rect({ Length, Thickness });
				Rect.setOrigin({ Length / 2.0f, Thickness / 2.0f });
				Rect.setFillColor(color::Transparent);
				Rect.setOutlineThickness(2.0f);
				Rect.setOutlineColor(color::White);  // Static body color

				DebugRects.push_back(Rect);
			}

			bDebugInitialized = true;
		}

		// Update positions and add to drawables
		size_t SegmentCount = bClosed ? Points.size() : Points.size() - 1;
		for (size_t i = 0; i < SegmentCount; ++i)
		{
			vec2f A = Points[i];
			vec2f B = Points[(i + 1) % Points.size()];

			vec2f Midpoint = (A + B) / 2.0f;
			vec2f Delta = B - A;
			float Angle = std::atan2(Delta.y, Delta.x);

			DebugRects[i].setPosition(Midpoint);
			DebugRects[i].setRotation(sf::radians(Angle));

			OutDrawables.push_back(&DebugRects[i]);
		}
	}
}
