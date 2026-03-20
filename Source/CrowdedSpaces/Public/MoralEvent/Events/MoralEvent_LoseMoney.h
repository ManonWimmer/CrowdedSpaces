#pragma once

#include "CoreMinimal.h"
#include "MoralEvent/MoralEvent.h"
#include "MoralEvent_LoseMoney.generated.h"

UCLASS()
class CROWDEDSPACES_API UMoralEvent_LoseMoney : public UMoralEvent
{
	GENERATED_BODY()

public:
	virtual void ClickOnChoice(EMoralEventType Choice) override;

	virtual void SetupChoices() override;

private:
	UPROPERTY(EditAnywhere)
	float MoneyToLose = 100;
};
