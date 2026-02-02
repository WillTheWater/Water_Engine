// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Utility/CoreMinimal.h"

namespace we
{
	class Object : public std::enable_shared_from_this<Object>
	{
	public:
		Object();
		virtual ~Object();

		virtual void Destroy();
		bool IsPendingDestroy() const;

		weak<Object> GetObject();
		weak<const Object> GetObject() const;

		//Delegate<Object*> OnDestroy;

	private:
		bool bIsPendingDestroy;
	};
}