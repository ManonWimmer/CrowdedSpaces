#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Resources/ResourceType.h"
#include "ResourceComponent.generated.h"

class UCrowdedGameInstance;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResourceFull); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourceChanged, int32, Value); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResourceAdded); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResourceRemoved); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxResourceChanged, int32, Value); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsInRegenChanged, bool, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNoMoreResource); 

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWDEDSPACES_API UResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UResourceComponent();

	// Type
	void SetType(EResourceType NewType);

	UFUNCTION(BlueprintCallable)
	EResourceType GetType() const { return ResourceType; }
	
	// Change value
	UFUNCTION(BlueprintCallable)
	void AddResource(float Amount);

	UFUNCTION(BlueprintCallable)
	void RemoveResource(float Amount);

	UFUNCTION(BlueprintCallable)
	void AddMaxResource(float Amount);

	UFUNCTION(BlueprintCallable)
	void RemoveMaxResource(float Amount);

	// Get value
	UFUNCTION(BlueprintCallable)
	float GetResource() const { return Resource; }

	UFUNCTION(BlueprintCallable)
	float GetMaxResource() const { return MaxResource; }

	// Check value
	UFUNCTION(BlueprintCallable)
	bool HasEnoughResource(float  Amount);
	
	UFUNCTION(BlueprintCallable)
	bool HasMaxResource();
	
	// Timer control
	UFUNCTION(BlueprintCallable)
	void StartResourceTimer();

	UFUNCTION(BlueprintCallable)
	void ToggleResourceTimer();
	
	UFUNCTION(BlueprintCallable)
	void StopResourceTimer();

	// Using
	UFUNCTION(BlueprintCallable)
	void SetCanLoseAndRegenResource(bool bCanLoseAndRegen);
	
	UFUNCTION(BlueprintCallable)
	void SetIsInRegen(bool bInRegen);
	
	UFUNCTION(BlueprintCallable)
	bool GetIsInRegen() const { return bIsInRegen; }

	UFUNCTION(BlueprintCallable)
	void SetResourceRegenPerTick(const float NewRegen) { ResourceRegenPerTick = NewRegen; }

	UFUNCTION(BlueprintCallable)
	void SetResourceLossPerTick(const float NewRegen) { ResourceLossPerTick = NewRegen; }

	// Delegates
	UPROPERTY(BlueprintAssignable)
	FOnResourceFull OnResourceFull;

	UPROPERTY(BlueprintAssignable)
	FOnResourceChanged OnResourceChanged;

	UPROPERTY(BlueprintAssignable)
	FOnResourceAdded OnResourceAdded;

	UPROPERTY(BlueprintAssignable)
	FOnResourceRemoved OnResourceRemoved;

	UPROPERTY(BlueprintAssignable)
	FOnIsInRegenChanged OnIsInRegenChanged;

	UPROPERTY(BlueprintAssignable)
	FOnNoMoreResource OnNoMoreResource;

	UPROPERTY(BlueprintAssignable)
	FOnMaxResourceChanged OnMaxResourceChanged;
	
private:
	// Resource
	UPROPERTY(EditAnywhere)
	float Resource = 0;

	UPROPERTY(EditAnywhere)
	float MaxResource = 0;

	UPROPERTY(EditAnywhere)
	bool bSetMaxResourceAtStart = true;

	UPROPERTY(EditAnywhere)
	bool bIsInRegen = false;

	// Loss & regen
	bool CanLoseAndRegenResource = false;
	
	UPROPERTY(EditAnywhere)
	float ResourceLossPerTick = 1;

	UPROPERTY(EditAnywhere)
	float ResourceRegenPerTick = 2;

	// Timer
	UPROPERTY(EditAnywhere)
	float TickInterval = 1.f;

	FTimerHandle ResourceTimerHandle;

	void ResourceTick();

	// Type
	UPROPERTY(EditAnywhere)
	EResourceType ResourceType = EResourceType::Money;

	UPROPERTY()
	TObjectPtr<UCrowdedGameInstance> GameInstance = nullptr;

	UPROPERTY()
	TObjectPtr<UResourceComponent> PlayerFoodForRegen = nullptr;

protected:
	virtual void BeginPlay() override;
};
