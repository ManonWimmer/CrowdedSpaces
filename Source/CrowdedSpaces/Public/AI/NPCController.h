#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "NPCController.generated.h"

UCLASS()
class CROWDEDSPACES_API ANPCController : public AAIController
{
	GENERATED_BODY()

public:
	explicit ANPCController(FObjectInitializer const& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;
};
