#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "Resources/FoodComponent.h"
#include "Components/WidgetComponent.h"
#include "Resources/OxygenComponent.h"
#include "Resources/EnergyComponent.h"
#include "Selection/Selectable.h"
#include "NPCAction.h"
#include "NPC.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentActionChanged, ENPCAction, Value); 

UCLASS()
class CROWDEDSPACES_API ANPC : public ACharacter, public ISelectable
{
	GENERATED_BODY()

public:
	ANPC();

	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

	UFUNCTION(BlueprintCallable, Category="AI")
	UFoodComponent* GetFoodComponent() const { return FoodComponent; }

	UFUNCTION(BlueprintCallable, Category="AI")
	UEnergyComponent* GetEnergyComponent() const { return EnergyComponent; }

	UFUNCTION(BlueprintCallable, Category="AI")
	void SetCurrentAction(ENPCAction NewAction);

	UFUNCTION(BlueprintCallable)
	ENPCAction GetCurrentAction() const { return CurrentAction; }

	UPROPERTY(BlueprintAssignable)
	FOnCurrentActionChanged OnCurrentActionChanged;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

private:
	// Food
	UPROPERTY(EditAnywhere)
	TObjectPtr<UFoodComponent> FoodComponent;
	
	UPROPERTY(EditAnywhere, Category="Food")
	float RemoveFoodInterval = 1.0f;

	UPROPERTY(EditAnywhere, Category="Food")
	int32 RemoveFoodPerInterval = 10;

	// Oxygen
	UPROPERTY(EditAnywhere)
	TObjectPtr<UOxygenComponent> OxygenComponent;

	UFUNCTION()
	void RemoveFood() const;

	UPROPERTY()
	FTimerHandle RemoveFoodTimerHandle;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> FoodBarWidget;

	// Energy
	UPROPERTY(EditAnywhere)
	TObjectPtr<UEnergyComponent> EnergyComponent;

	// Action
	UPROPERTY()
	ENPCAction CurrentAction = ENPCAction::Idle;
	
	// Selectable
public:
	virtual void OnSelected() override;
	virtual void OnDeselected() override;
};
