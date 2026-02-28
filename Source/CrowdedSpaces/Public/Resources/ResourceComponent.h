#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Resources/ResourceType.h"
#include "ResourceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResourceFull); 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourceChanged, int32, Value); 
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
	EResourceType GetType() { return ResourceType;}
	
	// Change value
	UFUNCTION(BlueprintCallable)
	void AddResource(int Amount);

	UFUNCTION(BlueprintCallable)
	void RemoveResource(int Amount);

	// Get value
	UFUNCTION(BlueprintCallable)
	int GetResource() const { return Resource; }

	UFUNCTION(BlueprintCallable)
	int GetMaxResource() const { return MaxResource; }

	// Check value
	UFUNCTION(BlueprintCallable)
	bool HasEnoughResource(int Amount);
	
	UFUNCTION(BlueprintCallable)
	bool HasMaxResource();
	
	// Timer control
	UFUNCTION(BlueprintCallable)
	void StartResourceTimer();

	UFUNCTION(BlueprintCallable)
	void StopResourceTimer();

	// Using
	UFUNCTION(BlueprintCallable)
	void SetCanLoseAndRegenResource(bool bCanLoseAndRegen);
	
	UFUNCTION(BlueprintCallable)
	void SetIsInRegen(bool bInRegen);
	
	UFUNCTION(BlueprintCallable)
	bool GetIsInRegen() const { return bIsInRegen; }

	// Delegates
	UPROPERTY(BlueprintAssignable)
	FOnResourceFull OnResourceFull;

	UPROPERTY(BlueprintAssignable)
	FOnResourceChanged OnResourceChanged;

	UPROPERTY(BlueprintAssignable)
	FOnIsInRegenChanged OnIsInRegenChanged;

	UPROPERTY(BlueprintAssignable)
	FOnNoMoreResource OnNoMoreResource;
	
private:
	// Resource
	UPROPERTY(EditAnywhere)
	int32 Resource = 0;

	UPROPERTY(EditAnywhere)
	int32 MaxResource = 0;

	UPROPERTY(EditAnywhere)
	bool bIsInRegen = false;

	// Loss & regen
	bool CanLoseAndRegenResource = false;
	
	UPROPERTY(EditAnywhere)
	int32 ResourceLossPerTick = 1;

	UPROPERTY(EditAnywhere)
	int32 ResourceRegenPerTick = 2;

	// Timer
	UPROPERTY(EditAnywhere)
	float TickInterval = 1.f;

	FTimerHandle ResourceTimerHandle;

	void ResourceTick();

	// Type
	UPROPERTY(EditAnywhere)
	EResourceType ResourceType = EResourceType::Money;

protected:
	virtual void BeginPlay() override;
};
