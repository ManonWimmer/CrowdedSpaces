#include "AI/NPCFunctionLibrary.h"
#include "AI/NPC.h"
#include "Game/CrowdedGameInstance.h"

ANPC* UNPCFunctionLibrary::SpawnGlobalNPC(UObject* WorldContextObject)
{
	UWorld* World = WorldContextObject->GetWorld();
	if (!World)
		return nullptr;

	UCrowdedGameInstance* GameInstance = World->GetGameInstance<UCrowdedGameInstance>();
	if (!GameInstance)
		return nullptr;

	const FVector Location(0.0f, 0.0f, 140.0f);
	const FRotator Rotation(0.0f, 0.0f, 0.0f);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	return World->SpawnActor<ANPC>(GameInstance->GlobalNPCClass, Location, Rotation, SpawnInfo);
}
