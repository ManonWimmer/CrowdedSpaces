#pragma once

#include "CoreMinimal.h"

UCLASS(BlueprintType)
class CROWDEDSPACES_API UMoralEvent : public UObject
{
	GENERATED_BODY();

public:
	UFUNCTION(BlueprintCallable)
	virtual void StartMoralEvent();

	UFUNCTION(BlueprintCallable)
	virtual void ClickOnChoice(int choiceIndex);

	UFUNCTION()
	virtual void SetupChoices();
	
	// titre

	// description

	UPROPERTY(EditAnywhere)
	TArray<FString> Choices;
};
