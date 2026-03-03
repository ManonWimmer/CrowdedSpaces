#pragma once

#include "CoreMinimal.h"
#include "AI/NPC.h"
#include "MoralEvent/MoralEvent.h"
#include "MoralEvent_RandomNPCDeath.generated.h"

UCLASS(Blueprintable)
class CROWDEDSPACES_API UMoralEvent_RandomNPCDeath : public UMoralEvent
{
	GENERATED_BODY()

public:
	virtual void ClickOnChoice(EMoralEventType Choice) override;

	virtual void SetupChoices() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ANPC> NPCClass;

	UPROPERTY()
	TObjectPtr<ANPC> SelectedNPC = nullptr;
};
