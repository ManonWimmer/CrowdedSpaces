#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_IsTired.generated.h"

UCLASS()
class CROWDEDSPACES_API UBTDecorator_IsTired : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTDecorator_IsTired(FObjectInitializer const& ObjectInitializer);

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category="Condition")
	int32 TiredUnderEnergy = 30;

private:
	bool CachedValue = false;
};

