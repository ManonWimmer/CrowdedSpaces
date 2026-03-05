#pragma once

#include "CoreMinimal.h"
#include "MoralEvent/MoralEvent.h"
#include "MoralEvent_LoseFood.generated.h"

UCLASS()
class CROWDEDSPACES_API UMoralEvent_LoseFood : public UMoralEvent
{
	GENERATED_BODY()

public:
	virtual void ClickOnChoice(EMoralEventType Choice) override;

	virtual void SetupChoices() override;

private:
	UPROPERTY(EditAnywhere)
	float FoodToLose = 100;
};
