#pragma once

#include "CoreMinimal.h"
#include "AI/CustomBTTask.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_UseBuildableObject.generated.h"

class UEnergyComponent;

UCLASS()
class CROWDEDSPACES_API UBTTask_UseBuildableObject : public UCustomBTTask
{
	GENERATED_BODY()

public:
	explicit UBTTask_UseBuildableObject(FObjectInitializer const& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	UFUNCTION()
	void OnStopAction();

	UFUNCTION()
	void OnTargetDestroyed() const;

	UFUNCTION()
	void ForceStopTask() const;
	
private:
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetObjectKey;
	
	TWeakObjectPtr<UBehaviorTreeComponent> OwnerCompPtr;
};
