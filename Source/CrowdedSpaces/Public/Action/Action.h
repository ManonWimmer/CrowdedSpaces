#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Action.generated.h"

class UActionSubsystem;

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class CROWDEDSPACES_API UAction : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void Initialize(const UWorld* World);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Action")
	FString ActionName;

	UFUNCTION(BlueprintNativeEvent)
	bool CanExecute(AActor* Instigator) const;

	virtual bool CanExecute_Implementation(AActor* Instigator) const;

	UFUNCTION(BlueprintNativeEvent)
	void Execute(AActor* Instigator);

	virtual void Execute_Implementation(AActor* Instigator);

	UFUNCTION(BlueprintCallable)
	void ExecuteWrapper(AActor* Instigator);

	UFUNCTION(BlueprintCallable)
	bool CanExecuteWrapper(AActor* Instigator) const;

protected:
	UPROPERTY()
	TObjectPtr<UActionSubsystem> ActionSubsystem;
};
