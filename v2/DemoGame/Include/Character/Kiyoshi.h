// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Character/NPC.h"

namespace we
{
	// =============================================================================
	// Kiyoshi - Old Man Merchant NPC with 8-way idle animation
	// =============================================================================

	class Kiyoshi : public NPC
	{
	public:
		Kiyoshi(World* OwningWorld);

	protected:
		virtual void InitializeAnimations() override;
	};
}
