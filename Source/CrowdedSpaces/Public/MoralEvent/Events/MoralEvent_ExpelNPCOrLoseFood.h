#pragma once

#include "CoreMinimal.h"
#include "AI/NPC.h"
#include "MoralEvent/MoralEvent.h"
#include "MoralEvent_ExpelNPCOrLoseFood.generated.h"

UCLASS()
class CROWDEDSPACES_API UMoralEvent_ExpelNPCOrLoseFood : public UMoralEvent
{
	GENERATED_BODY()

public:
	virtual void ClickOnChoice(EMoralEventType Choice) override;

	virtual void SetupChoices() override;

private:
	UPROPERTY(EditAnywhere)
	float FoodToLose = 100;
};
