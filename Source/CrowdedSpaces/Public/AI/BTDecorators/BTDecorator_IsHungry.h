#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_IsHungry.generated.h"

UCLASS()
class CROWDEDSPACES_API UBTDecorator_IsHungry : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTDecorator_IsHungry(FObjectInitializer const& ObjectInitializer);

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category="Condition")
	int32 HungryUnderFood = 30;

private:
	bool CachedValue = false;
};

