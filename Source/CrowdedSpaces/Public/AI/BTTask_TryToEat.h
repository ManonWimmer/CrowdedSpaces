#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_TryToEat.generated.h"

UCLASS()
class CROWDEDSPACES_API UBTTask_TryToEat: public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_TryToEat(FObjectInitializer const& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
