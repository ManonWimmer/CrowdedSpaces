#pragma once

#include "CoreMinimal.h"
#include "MoralEventType.h"
#include "MoralEvent.generated.h"

UCLASS(Blueprintable, BlueprintType)
class CROWDEDSPACES_API UMoralEvent : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void ClickOnChoice(EMoralEventType Choice);

	UFUNCTION()
	virtual void SetupChoices();

	UFUNCTION(BlueprintCallable)
	TArray<EMoralEventType> GetChoices() { return Choices; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MoralEvent")
	FString EventTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MoralEvent")
	FString EventDescription;

	UPROPERTY()
	TArray<EMoralEventType> Choices;
};
