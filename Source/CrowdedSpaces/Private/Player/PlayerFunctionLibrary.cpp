#include "Player/PlayerFunctionLibrary.h"

#include "Resources/ResourceType.h"
#include "Resources/ResourceComponent.h"
#include "Game/CrowdedGameState.h"

UResourceComponent* UPlayerFunctionLibrary::GetPlayerResourceComponent(UObject* WorldContextObject, const EResourceType ResourceType)
{
	// to do: init game state avant pour pas le refaire à chaque fois
	const UWorld* World = WorldContextObject->GetWorld();
	if (!World)
		return nullptr;
	
	const TObjectPtr<ACrowdedGameState> GameState = World->GetGameState<ACrowdedGameState>();
	if (!GameState)
		return nullptr;

	return GameState->GetResourceComponentByType(ResourceType);
}
