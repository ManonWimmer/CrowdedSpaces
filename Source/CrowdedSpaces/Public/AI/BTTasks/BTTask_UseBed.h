#pragma once

#include "CoreMinimal.h"
#include "AI/CustomBTTask.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_UseBed.generated.h"

class UEnergyComponent;

UCLASS()
class CROWDEDSPACES_API UBTTask_UseBed : public UCustomBTTask
{
	GENERATED_BODY()

public:
	explicit UBTTask_UseBed(FObjectInitializer const& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetBedKey;
	
   	TWeakObjectPtr<UBehaviorTreeComponent> OwnerCompPtr;
   	TWeakObjectPtr<UEnergyComponent> EnergyComp;

	UFUNCTION()
	void OnEnergyFull();
};
