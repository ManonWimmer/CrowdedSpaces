#include "Player/PlayerHelpers.h"

#include "Resources/ResourceType.h"
#include "Resources/ResourceComponent.h"
#include "Game/CrowdedGameState.h"

namespace PlayerHelpers
{
	UResourceComponent* GetPlayerResourceComponent(UWorld& World,
	const EResourceType ResourceType)
	{
		const TObjectPtr<ACrowdedGameState> GameState = World.GetGameState<ACrowdedGameState>();
		if (!GameState)
			return nullptr;

		return GameState->GetResourceComponentByType(ResourceType);
	}
}

