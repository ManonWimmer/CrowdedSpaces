#pragma once

#include "CoreMinimal.h"
#include "AI/CustomBTTask.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindNearestAvailableBed.generated.h"

UCLASS()
class CROWDEDSPACES_API UBTTask_FindNearestAvailableBed : public UCustomBTTask
{
	GENERATED_BODY()
	
public:
	explicit UBTTask_FindNearestAvailableBed(FObjectInitializer const& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	UPROPERTY(EditAnywhere, Category = "AI")
	float SearchRadius = 500.f;

	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetLocationKey;
	
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetBedKey;
};
