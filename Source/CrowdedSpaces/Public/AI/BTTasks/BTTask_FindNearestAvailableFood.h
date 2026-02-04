#pragma once

#include "CoreMinimal.h"
#include "AI/CustomBTTask.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindNearestAvailableFood.generated.h"

UCLASS()
class CROWDEDSPACES_API UBTTask_FindNearestAvailableFood : public UCustomBTTask
{
	GENERATED_BODY()
	
public:
	explicit UBTTask_FindNearestAvailableFood(FObjectInitializer const& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	UPROPERTY(EditAnywhere, Category = "AI")
	float SearchRadius = 500.f;

	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetLocationKey;
	
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetFoodKey;
};
