// =============================================================================
// Water Engine v2.0.0
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	class Object : public std::enable_shared_from_this<Object>
	{
	public:
		Object();
		virtual ~Object();

		bool IsPendingDestroy() const;
		void Destroy();

		weak<Object> GetObject();
		weak<const Object> GetObject() const;

	protected:

	private:
		bool bIsPendingDestroy;
	};
}