#pragma once

#include "CoreMinimal.h"
#include "MoralEventType.h"
#include "AI/NPC.h"
#include "Engine/DataAsset.h"
#include "MoralEventData.generated.h"

UCLASS()
class CROWDEDSPACES_API UMoralEventData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMoralEventType Type;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ANPC> NPCClass;
};
