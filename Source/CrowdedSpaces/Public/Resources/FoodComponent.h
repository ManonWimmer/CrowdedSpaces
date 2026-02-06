#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Selection/SelectableStatProvider.h"
#include "FoodComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFoodFull); // To know when to stop eating
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFoodChanged, int32, Value); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsEatingChanged, bool, Value); 

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWDEDSPACES_API UFoodComponent : public UActorComponent, public ISelectableStatProvider
{
	GENERATED_BODY()

public:
	UFoodComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AddFood(int Amount);

	UFUNCTION(BlueprintCallable)
	void RemoveFood(int Amount);

	UFUNCTION(BlueprintCallable)
	bool HasEnoughFood(int Amount);

	// Timer control
	UFUNCTION(BlueprintCallable)
	void StartFoodTimer();

	UFUNCTION(BlueprintCallable)
	void StopFoodTimer();

	UFUNCTION(BlueprintCallable)
	void SetEating(bool bEating);

	UFUNCTION(BlueprintCallable)
	int GetFood() const { return Food; }

	UFUNCTION(BlueprintCallable)
	int GetMaxFood() const { return MaxFood; }

	UFUNCTION(BlueprintCallable)
	bool GetIsEating() const { return bIsEating; }

	UPROPERTY(BlueprintAssignable)
	FOnFoodFull OnFoodFull;

	UPROPERTY(BlueprintAssignable)
	FOnFoodChanged OnFoodChanged;

	UPROPERTY(BlueprintAssignable)
	FOnIsEatingChanged OnIsEatingChanged;

	UFUNCTION(BlueprintCallable)
	void SetSufferHunger(bool bHunger) { bSufferHunger = bHunger; }
	
	// Selectable
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnStatChanged;
	
	virtual TArray<FStat> GetCurrentValues() const override;
	virtual FOnStatChanged& GetOnStatChanged() override { return OnStatChanged; }
	
private:
	UPROPERTY(EditAnywhere)
	int32 Food = 0;

	UPROPERTY(EditAnywhere)
	int32 MaxFood = 100;

	UPROPERTY(EditAnywhere, Category="Food")
	int32 FoodLossPerTick = 1;

	UPROPERTY(EditAnywhere, Category="Food")
	int32 FoodRegenPerTick = 2;

	UPROPERTY(EditAnywhere, Category="Food")
	float TickInterval = 1.f;

	bool bIsEating = false;

	UPROPERTY(EditAnywhere)
	bool bSufferHunger = false;

	FTimerHandle FoodTimerHandle;

	void FoodTick();
};
