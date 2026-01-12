#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Selection/SelectableStatProvider.h"
#include "OxygenComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWDEDSPACES_API UOxygenComponent : public UActorComponent, public ISelectableStatProvider
{
	GENERATED_BODY()

public:
	UOxygenComponent();

	UFUNCTION(BlueprintCallable)
	void AddOxygen(int Amount);

	UFUNCTION(BlueprintCallable)
	void RemoveOxygen(int Amount);

	UFUNCTION(BlueprintCallable)
	bool HasEnoughOxygen(int Amount);

	UFUNCTION(BlueprintCallable)
	int GetOxygen() {return Oxygen; }

	// Selectable
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnStatChanged;
	
	virtual FString GetStatDisplayName() const override{ return "Oxygen";}
	virtual float GetCurrentValue() const override { return Oxygen; }
	virtual FOnStatChanged& GetOnStatChanged() override { return OnStatChanged; }
	
private:
	UPROPERTY(EditAnywhere)
	int32 Oxygen = 0;
};
