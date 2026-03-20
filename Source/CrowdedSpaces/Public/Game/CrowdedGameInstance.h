#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CrowdedGameInstance.generated.h"

class ANPC;

UCLASS()
class CROWDEDSPACES_API UCrowdedGameInstance : public UGameInstance
{
	GENERATED_BODY()

	virtual void Init() override;

	void OnPostWorldInitialization(UWorld* World, const UWorld::InitializationValues IVS);

public:
	UFUNCTION()
	void ResetGameSettings(); // Called by game mode begin play
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bLastGameSurvived = false;

	UPROPERTY(BlueprintReadWrite)
	bool bNPCsCanLoseFood = true;

	UPROPERTY(BlueprintReadWrite)
	bool bNPCsCanLoseEnergy = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="NPC")
	TSubclassOf<ANPC> GlobalNPCClass;
};
