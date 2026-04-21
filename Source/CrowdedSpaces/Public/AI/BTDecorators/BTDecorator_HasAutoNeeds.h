#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_HasAutoNeeds.generated.h"

UCLASS()
class CROWDEDSPACES_API UBTDecorator_HasAutoNeeds : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTDecorator_HasAutoNeeds(FObjectInitializer const& ObjectInitializer);

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool CachedValue = false;
};
