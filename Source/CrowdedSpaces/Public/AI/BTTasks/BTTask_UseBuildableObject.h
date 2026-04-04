#pragma once

#include "CoreMinimal.h"
#include "AI/CustomBTTask.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Resources/ResourceType.h"
#include "BTTask_UseBuildableObject.generated.h"

class ANPC;
class ABuildableObject;

UCLASS()
class UBTTask_UseBuildableObject : public UCustomBTTask
{
	GENERATED_BODY()

public:
	explicit UBTTask_UseBuildableObject(FObjectInitializer const& ObjectInitializer);
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	void StopUsingClean() const;

protected:
	UPROPERTY()
	TObjectPtr<ABuildableObject> CurrentObject = nullptr;

	UPROPERTY(EditAnywhere)
	EResourceType ResourceTypeToCheck;

	bool bHasStartedUsing = false;
};