#pragma once

#include "CoreMinimal.h"
#include "AI/CustomBTTask.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Build/ObjectType.h"
#include "BTTask_FindNearestAvailableBuildableObject.generated.h"

UCLASS()
class CROWDEDSPACES_API UBTTask_FindNearestAvailableBuildableObject : public UCustomBTTask
{
	GENERATED_BODY()
	
public:
	explicit UBTTask_FindNearestAvailableBuildableObject(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
private:
	UPROPERTY(EditAnywhere, Category = "AI")
	float SearchRadius = 500.f;

	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetLocationKey;
	
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetObjectKey;

	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetSlotKey;

	UPROPERTY(EditAnywhere, Category="Blackboard")
	EObjectType BuildableObjectType = EObjectType::Default;
};
