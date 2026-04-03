#pragma once

#include "CoreMinimal.h"
#include "AI/CustomBTTask.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Resources/ResourceType.h"
#include "BTTask_ReserveBuildableSlot.generated.h"

class ANPC;
class ABuildableObject;

UCLASS()
class UBTTask_ReserveBuildableSlot : public UCustomBTTask
{
	GENERATED_BODY()

public:
	explicit UBTTask_ReserveBuildableSlot(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetObjectKey;

	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetSlotKey;
};