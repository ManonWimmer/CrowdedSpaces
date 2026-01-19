#pragma once

#include "CoreMinimal.h"
#include "UI/CustomWidget.h"
#include "MoralEvent/MoralEventData.h"
#include "MoralEventWidget.generated.h"

UCLASS()
class CROWDEDSPACES_API UMoralEventWidget : public UCustomWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	virtual void Init_Implementation() override;
	virtual void Reset_Implementation() override;

public:
	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void Update(const UMoralEventData* EventData);
};
