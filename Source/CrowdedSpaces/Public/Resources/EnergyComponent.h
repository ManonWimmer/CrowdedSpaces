#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Selection/SelectableStatProvider.h"
#include "EnergyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnergyFull); // To know when to get out of bed ;)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWDEDSPACES_API UEnergyComponent : public UActorComponent, public ISelectableStatProvider
{
	GENERATED_BODY()

public:
	UEnergyComponent();

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void AddEnergy(int Amount);

	UFUNCTION(BlueprintCallable)
	void RemoveEnergy(int Amount);

	UFUNCTION(BlueprintCallable)
	bool HasEnoughEnergy(int Amount);
	
	UFUNCTION(BlueprintCallable)
	bool HasMaxEnergy();
	
	// Timer control
	UFUNCTION(BlueprintCallable)
	void StartEnergyTimer();

	UFUNCTION(BlueprintCallable)
	void StopEnergyTimer();

	UFUNCTION(BlueprintCallable)
	void SetSleeping(bool bSleeping);
	
	UFUNCTION(BlueprintCallable)
	bool GetIsSleeping() { return bIsSleeping; }
	
	UFUNCTION(BlueprintCallable)
	int GetEnergy() {return Energy; }

	UPROPERTY(BlueprintAssignable)
	FOnEnergyFull OnEnergyFull;
	
	// Selectable
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnStatChanged;
	
	virtual TArray<FStat> GetCurrentValues() const override;
	virtual FOnStatChanged& GetOnStatChanged() override { return OnStatChanged; }
	
private:
	UPROPERTY(EditAnywhere, Category="Energy")
	int32 Energy = 100;

	UPROPERTY(EditAnywhere, Category="Energy")
	int32 MaxEnergy = 100;

	UPROPERTY(EditAnywhere, Category="Energy")
	int32 EnergyLossPerTick = 1;

	UPROPERTY(EditAnywhere, Category="Energy")
	int32 EnergyRegenPerTick = 2;

	UPROPERTY(EditAnywhere, Category="Energy")
	float TickInterval = 1.f;

	bool bIsSleeping = false;

	FTimerHandle EnergyTimerHandle;

	void EnergyTick();
};
