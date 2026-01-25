#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_UseBed.generated.h"

class UEnergyComponent;

UCLASS()
class CROWDEDSPACES_API UBTTask_UseBed : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTTask_UseBed(FObjectInitializer const& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetBedKey;
	
   	TWeakObjectPtr<UBehaviorTreeComponent> OwnerCompPtr;
   	TWeakObjectPtr<UEnergyComponent> EnergyComp;

	UFUNCTION()
	void OnEnergyFull();
};
