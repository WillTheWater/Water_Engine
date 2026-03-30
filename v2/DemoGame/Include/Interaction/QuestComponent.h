// =============================================================================
// Water Engine v2.0.0 - Demo Game
// Copyright(C) 2026 Will The Water
// =============================================================================

#pragma once

#include "Core/CoreMinimal.h"

namespace we
{
	struct FQuestState
	{
		bool bMetAoi = false;
		bool bFoundGrandpa = false;
		bool bKiyoshiItemFound = false;
		bool bCanExitForest = false;
	};

	class QuestComponent
	{
	public:
		QuestComponent() = default;
		~QuestComponent() = default;

		// Getters
		bool HasMetAoi() const { return State.bMetAoi; }
		bool HasFoundGrandpa() const { return State.bFoundGrandpa; }
		bool HasKiyoshiItem() const { return State.bKiyoshiItemFound; }
		bool CanExitForest() const { return State.bCanExitForest; }
		const FQuestState& GetState() const { return State; }

		// Progression
		void MarkMetAoi() { State.bMetAoi = true; }
		void MarkFoundGrandpa() { State.bFoundGrandpa = true; }
		void MarkItemFound() { State.bKiyoshiItemFound = true; }
		void MarkCanExit() { State.bCanExitForest = true; }

		// Utility
		bool IsQuestComplete() const { return State.bCanExitForest; }

	private:
		FQuestState State;
	};
}
