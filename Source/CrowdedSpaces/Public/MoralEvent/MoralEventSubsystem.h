#pragma once

#include "CoreMinimal.h"
#include "MoralEventData.h"
#include "Subsystems/WorldSubsystem.h"
#include "UI/GameHUD.h"
#include "MoralEventSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNoSelected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnYesSelected);

UCLASS()
class CROWDEDSPACES_API UMoralEventSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

//to do: get datas of moral event data type to select random ? 
	
public:
	UFUNCTION(BlueprintCallable, Category="MoralEventManager")
	void StartNewEvent(UMoralEventData* EventData);

	UFUNCTION(BlueprintCallable, Category="MoralEventManager")
	void EndCurrentEvent(bool bSelected);

	UPROPERTY(BlueprintAssignable)
	FOnNoSelected OnNoSelected;

	UPROPERTY(BlueprintAssignable)
	FOnYesSelected OnYesSelected;
	
private:
	void GetGameHUD();
	
	UPROPERTY()
	UMoralEventData* CurrentEventData = nullptr;

// todo : use moral events get in game state comme build ? 
	
	UPROPERTY()
	AGameHUD* GameHUD = nullptr;
};
