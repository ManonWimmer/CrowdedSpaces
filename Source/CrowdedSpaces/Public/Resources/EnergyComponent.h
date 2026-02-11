#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnergyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnergyFull); // To know when to get out of bed ;)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnergyChanged, int32, Value); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsSleepingChanged, bool, Value); 

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWDEDSPACES_API UEnergyComponent : public UActorComponent
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
	bool GetIsSleeping() const { return bIsSleeping; }
	
	UFUNCTION(BlueprintCallable)
	int GetEnergy() const { return Energy; }

	UFUNCTION(BlueprintCallable)
	int GetMaxEnergy() const { return MaxEnergy; }

	UPROPERTY(BlueprintAssignable)
	FOnEnergyFull OnEnergyFull;

	UPROPERTY(BlueprintAssignable)
	FOnEnergyChanged OnEnergyChanged;

	UPROPERTY(BlueprintAssignable)
	FOnIsSleepingChanged OnIsSleepingChanged;
	
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
