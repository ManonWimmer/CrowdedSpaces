#pragma once

#include "CoreMinimal.h"
#include "MoralEvent/MoralEvent.h"
#include "MoralEvent_SpawnNPC.generated.h"

UCLASS()
class CROWDEDSPACES_API UMoralEvent_SpawnNPC : public UMoralEvent
{
	GENERATED_BODY()

public:
	virtual void ClickOnChoice(EMoralEventType Choice) override;

	virtual void SetupChoices() override;
};
