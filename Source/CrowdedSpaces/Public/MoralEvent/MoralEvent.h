#pragma once

#include "CoreMinimal.h"
#include "MoralEvent.generated.h"

UCLASS(Blueprintable, BlueprintType)
class CROWDEDSPACES_API UMoralEvent : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void ClickOnChoice(int ChoiceIndex);

	UFUNCTION()
	virtual void SetupChoices();

	UFUNCTION(BlueprintCallable)
	TArray<FString> GetChoices() { return Choices; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MoralEvent")
	FString EventTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MoralEvent")
	FString EventDescription;

	UPROPERTY()
	TArray<FString> Choices;
};
