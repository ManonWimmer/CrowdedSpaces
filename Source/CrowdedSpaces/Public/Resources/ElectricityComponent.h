#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Selection/SelectableStatProvider.h"
#include "ElectricityComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWDEDSPACES_API UElectricityComponent : public UActorComponent, public ISelectableStatProvider
{
	GENERATED_BODY()

public:
	UElectricityComponent();
	
	UFUNCTION(BlueprintCallable)
	void AddElectricity(int Amount);

	UFUNCTION(BlueprintCallable)
	void RemoveElectricity(int Amount);

	UFUNCTION(BlueprintCallable)
	bool HasEnoughElectricity(int Amount);
	
	UFUNCTION(BlueprintCallable)
	int GetElectricity() {return Electricity; }

	// Selectable
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnStatChanged;
	
	virtual TArray<TPair<FString, FString>> GetCurrentValues() const override;
	virtual FOnStatChanged& GetOnStatChanged() override { return OnStatChanged; }
	
private:
	UPROPERTY(EditAnywhere)
	int32 Electricity = 0;
};
