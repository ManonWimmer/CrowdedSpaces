#pragma once

#include "CoreMinimal.h"
#include "AI/CustomBTTask.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindRandomLocation.generated.h"

UCLASS()
class CROWDEDSPACES_API UBTTask_FindRandomLocation : public UCustomBTTask
{
	GENERATED_BODY()

public:
	explicit UBTTask_FindRandomLocation(FObjectInitializer const& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector RandomLocationKey;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float SearchRadius = 500.f;
};
