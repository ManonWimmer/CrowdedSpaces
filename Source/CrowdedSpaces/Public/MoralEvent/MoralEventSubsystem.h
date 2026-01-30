#pragma once

#include "CoreMinimal.h"
#include "MoralEvent.h"
#include "Subsystems/WorldSubsystem.h"
#include "UI/GameHUD.h"
#include "MoralEventSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMoralEventEnded);

UCLASS()
class CROWDEDSPACES_API UMoralEventSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="MoralEvent")
	void StartNewEvent(TSubclassOf<UMoralEvent> NewEvent);

	UFUNCTION(BlueprintCallable, Category="MoralEvent")
	void HandleRandomMoralEvent();

	UFUNCTION(BlueprintCallable, Category="MoralEvent")
	void OnChoiceSelected(EMoralEventType Choice);

	UFUNCTION(BlueprintCallable, Category="MoralEvent")
	void SetPossibleEvents(TArray<TSubclassOf<UMoralEvent>> NewPossibleEvents) { PossibleEvents = NewPossibleEvents; }

	UPROPERTY(BlueprintAssignable, Category = "MoralEvent")
	FOnMoralEventEnded OnMoralEventEnded;
	
private:
	void GetGameHUD();
	
	UPROPERTY()
	UMoralEvent* CurrentEvent = nullptr;

	UPROPERTY()
	TArray<TSubclassOf<UMoralEvent>> PossibleEvents; // Sent by game state
	
	UPROPERTY()
	AGameHUD* GameHUD = nullptr;
};
