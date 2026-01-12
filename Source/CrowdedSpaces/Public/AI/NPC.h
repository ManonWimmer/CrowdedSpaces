#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "Resources/FoodComponent.h"
#include "Components/WidgetComponent.h"
#include "Selection/Selectable.h"
#include "NPC.generated.h"

UCLASS()
class CROWDEDSPACES_API ANPC : public ACharacter, public ISelectable
{
	GENERATED_BODY()

public:
	ANPC();

	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

	UFUNCTION(BlueprintCallable, Category="AI")
	UFoodComponent* GetFoodComponent() const { return FoodComponent; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	UBehaviorTree* BehaviorTree;

private:
	// Food
	UPROPERTY(EditAnywhere)
	UFoodComponent* FoodComponent;
	
	UPROPERTY(EditAnywhere, Category="Food")
	float RemoveFoodInterval = 1.0f;

	UPROPERTY(EditAnywhere, Category="Food")
	int32 RemoveFoodPerInterval = 10;

	UFUNCTION()
	void RemoveFood() const;

	UFUNCTION()
	void StartRemoveFood();

	UPROPERTY()
	FTimerHandle RemoveFoodTimerHandle;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* FoodBarWidget;
	
	// Selectable
public:
	virtual void OnSelected() override;
	virtual void OnDeselected() override;

	virtual FString GetDisplayName() const override;

	virtual AActor* GetSelectableActor() override;
};
