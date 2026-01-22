#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_CheckNeedsToEat.generated.h"

UCLASS()
class CROWDEDSPACES_API UBTTask_CheckNeedsToEat: public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_CheckNeedsToEat(FObjectInitializer const& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "AI")
	float EatMinimum = 30.f;
};
