#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Resources/ResourceType.h"
#include "BTTask_UseBuildableObject.generated.h"

class ANPC;
class ABuildableObject;

UCLASS()
class UBTTask_UseBuildableObject : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTTask_UseBuildableObject();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY()
	ANPC* NPC = nullptr;

	UPROPERTY()
	ABuildableObject* CurrentObject = nullptr;

	UPROPERTY(EditAnywhere)
	EResourceType ResourceTypeToCheck;

	bool bHasStartedUsing = false;

	void StopUsingSafe();
};