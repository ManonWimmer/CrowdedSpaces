#pragma once

#include "CoreMinimal.h"
#include "AI/CustomBTTask.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveToWithReleaseSecurity.generated.h"

UCLASS()
class CROWDEDSPACES_API UBTTask_MoveToWithReleaseSecurity : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	explicit UBTTask_MoveToWithReleaseSecurity(FObjectInitializer const& ObjectInitializer);
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	void Cleanup(const UBehaviorTreeComponent& OwnerComp) const;

	UPROPERTY()
	TObjectPtr<ANPC> NPC;
};
