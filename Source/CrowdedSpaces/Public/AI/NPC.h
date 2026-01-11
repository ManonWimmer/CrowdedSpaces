#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "Resources/FoodComponent.h"
#include "Components/WidgetComponent.h"
#include "NPC.generated.h"

UCLASS()
class CROWDEDSPACES_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:
	ANPC();

	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	UBehaviorTree* BehaviorTree;

	UFUNCTION(BlueprintCallable, Category="AI")
	UFoodComponent* GetFoodComponent() const { return FoodComponent; }

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
};
