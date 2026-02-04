#pragma once

#include "CoreMinimal.h"
#include "AI/NPC.h"
#include "MoralEvent/MoralEvent.h"
#include "MoralEvent_ExpelNPC.generated.h"

UCLASS(Blueprintable)
class CROWDEDSPACES_API UMoralEvent_ExpelNPC : public UMoralEvent
{
	GENERATED_BODY()

public:
	virtual void ClickOnChoice(EMoralEventType Choice) override;

	virtual void SetupChoices() override;

private:
	UPROPERTY(EditAnywhere)
	float MoneyForExpel = 1000;
};
