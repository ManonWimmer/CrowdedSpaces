#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_IsTrainPriority.generated.h"

UCLASS()
class CROWDEDSPACES_API UBTDecorator_IsTrainPriority : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTDecorator_IsTrainPriority(FObjectInitializer const& ObjectInitializer);

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool CachedValue = false;
};
