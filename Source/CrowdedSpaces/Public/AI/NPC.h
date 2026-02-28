#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "Resources/ResourceComponent.h"
#include "Components/WidgetComponent.h"
#include "Selection/Selectable.h"
#include "NPCAction.h"
#include "Production/ProductionType.h"
#include "NPC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentActionChanged, ENPCActionWidget, Value); 

UCLASS()
class CROWDEDSPACES_API ANPC : public ACharacter, public ISelectable
{
	GENERATED_BODY()

public:
	ANPC();

	// Resource
	UFUNCTION(BlueprintCallable)
	UResourceComponent* GetResourceComponentByType(EResourceType Type) const;

	UFUNCTION(BlueprintCallable)
	int GetResourceByType(EResourceType Type) const;

	template <EResourceType Type>
	UResourceComponent* GetResourceComponent() const;

	template <EResourceType Type>
	int GetResource() const;

	// AI
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	
	UFUNCTION(BlueprintCallable, Category="AI")
	void SetCurrentAction(ENPCActionWidget NewAction);

	UFUNCTION(BlueprintCallable)
	ENPCActionWidget GetCurrentAction() const { return CurrentAction; }

	UPROPERTY(BlueprintAssignable)
	FOnCurrentActionChanged OnCurrentActionChanged;

	UFUNCTION()
	void Die();

	// Work
	UFUNCTION(BlueprintCallable, Category="AI")
	EProductionType GetWorkOnGeneratorType() const { return WorkOnGeneratorType; }

	UFUNCTION(BlueprintCallable, Category="AI")
	void SetWorkOnGeneratorType(EProductionType NewType);

	UFUNCTION(BlueprintCallable, Category="AI")
	int GetProductionMultiplierForType(EProductionType Type) const;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

private:
	UPROPERTY()
	TMap<EResourceType, TObjectPtr<UResourceComponent>> ResourceMap;
	
	// Food
	UPROPERTY(EditAnywhere)
	TObjectPtr<UResourceComponent> FoodComponent;
	
	UPROPERTY(EditAnywhere, Category="Food")
	float RemoveFoodInterval = 1.0f;

	UPROPERTY(EditAnywhere, Category="Food")
	int32 RemoveFoodPerInterval = 10;
	
	UFUNCTION()
	void RemoveFood() const;

	UPROPERTY()
	FTimerHandle RemoveFoodTimerHandle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> FoodBarWidget;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> NPCActionWidget;

	// Energy
	UPROPERTY(EditAnywhere)
	TObjectPtr<UResourceComponent> EnergyComponent;

	// Action
	UPROPERTY()
	ENPCActionWidget CurrentAction = ENPCActionWidget::Idle;

	// Work
	UPROPERTY(EditAnywhere)
	EProductionType WorkOnGeneratorType = EProductionType::Money;

	// Multipliers
	UPROPERTY()
	int FoodProductionMultiplier = 1;

	UPROPERTY()
	int ElectricityProductionMultiplier = 1;

	UPROPERTY()
	int MoneyProductionMultiplier = 1;
	
	// Selectable
public:
	virtual void OnSelected() override;
	virtual void OnDeselected() override;
};

template <EResourceType Type>
UResourceComponent* ANPC::GetResourceComponent() const
{
	return GetResourceComponentByType(Type);
}

template <EResourceType Type>
int ANPC::GetResource() const
{
	return GetResourceByType(Type);
}
