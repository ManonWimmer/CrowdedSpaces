#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CrowdedGameInstance.generated.h"

UCLASS()
class CROWDEDSPACES_API UCrowdedGameInstance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;

	void OnPostWorldInitialization(UWorld* World, const UWorld::InitializationValues IVS);
};
