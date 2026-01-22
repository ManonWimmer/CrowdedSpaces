#pragma once

#include "CoreMinimal.h"
#include "MoralEvent.h"
#include "Subsystems/WorldSubsystem.h"
#include "UI/GameHUD.h"
#include "MoralEventSubsystem.generated.h"

UCLASS()
class CROWDEDSPACES_API UMoralEventSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="MoralEventManager")
	void StartNewEvent(TSubclassOf<UMoralEvent> NewEvent);

	UFUNCTION(BlueprintCallable, Category="MoralEventManager")
	void OnChoiceSelected(EMoralEventChoice Choice);
	
private:
	void GetGameHUD();
	
	UPROPERTY()
	UMoralEvent* CurrentEvent = nullptr;

// todo : get moral events bp in game state (comme build) ? for random event
	
	UPROPERTY()
	AGameHUD* GameHUD = nullptr;
};
