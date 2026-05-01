#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_IsActionObjectSet.generated.h"

UCLASS()
class CROWDEDSPACES_API UBTDecorator_IsActionObjectSet : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTDecorator_IsActionObjectSet(FObjectInitializer const& ObjectInitializer);

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool CachedValue = false;
};
