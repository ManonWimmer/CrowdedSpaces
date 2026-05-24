#pragma once

#include "CoreMinimal.h"
#include "AI/CustomBTTask.h"
#include "BTTask_SetBlackboardBoolValue.generated.h"

UCLASS()
class CROWDEDSPACES_API UBTTask_SetBlackboardBoolValue : public UCustomBTTask
{
	GENERATED_BODY()

public:
	explicit UBTTask_SetBlackboardBoolValue(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetBoolKey;

	UPROPERTY(EditAnywhere, Category="Blackboard")
	bool bNewBoolValue;
};
