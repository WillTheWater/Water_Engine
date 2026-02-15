// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"
#include "Utility/Delegate.h"

namespace we
{
	class Object : public std::enable_shared_from_this<Object>
	{
	public:
		Object();
		virtual ~Object();

		bool IsPendingDestroy() const;
		uint GetID() const { return ObjectID; }

		Delegate<Object*> OnDestroy;

	protected:
		virtual void Destroy();

	private:
		bool bIsPendingDestroy;
		uint ObjectID;
		static uint NextID;
	};
}