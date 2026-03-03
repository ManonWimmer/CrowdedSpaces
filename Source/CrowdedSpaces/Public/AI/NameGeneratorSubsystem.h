#pragma once

#include "CoreMinimal.h"
#include "NameList.h"
#include "Subsystems/WorldSubsystem.h"
#include "NameGeneratorSubsystem.generated.h"

UCLASS()
class CROWDEDSPACES_API UNameGeneratorSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void SetNameData(UNameList* NewNameList);

	UFUNCTION(BlueprintCallable)
	FString GenerateName() const;
	bool IsInitialized() const { return bIsInitialized; }

private:
	bool bIsInitialized = false;
	
	UPROPERTY(EditAnywhere, Category="Names")
	UNameList* NameData; // Send by game state
};
