#pragma once

#include "CoreMinimal.h"
#include "AI/CustomBTTask.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_UseGenerator.generated.h"

UCLASS()
class CROWDEDSPACES_API UBTTask_UseGenerator : public UCustomBTTask
{
	GENERATED_BODY()

public:
	explicit UBTTask_UseGenerator(FObjectInitializer const& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetGeneratorKey;
};
