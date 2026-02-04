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

UCLASS()
class CROWDEDSPACES_API ANPC : public ACharacter, public ISelectable, public ISelectableStatProvider
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

	virtual FString GetDisplayName() const override;
	virtual TObjectPtr<AActor> GetSelectableActor() override;

	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnStatChanged; // Current action changed
	
	virtual TArray<FStat> GetCurrentValues() const override;
	virtual FOnStatChanged& GetOnStatChanged() override { return OnStatChanged; }
};
