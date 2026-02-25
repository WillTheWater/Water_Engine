// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Character/NPC.h"

namespace we
{
	// =============================================================================
	// Aoi - Village Girl NPC with simple 1-way idle animation
	// =============================================================================

	class Aoi : public NPC
	{
	public:
		Aoi(World* OwningWorld);

		void BeginPlay() override;

	protected:
		virtual void InitializeAnimations() override;
	};
}
