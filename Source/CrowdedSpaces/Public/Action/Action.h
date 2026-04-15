#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Action.generated.h"

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class CROWDEDSPACES_API UAction : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Action")
	FString ActionName;

	UFUNCTION(BlueprintNativeEvent)
	bool CanExecute(AActor* Instigator) const;

	virtual bool CanExecute_Implementation(AActor* Instigator) const;

	UFUNCTION(BlueprintNativeEvent)
	void Execute(AActor* Instigator);

	virtual void Execute_Implementation(AActor* Instigator);
};
