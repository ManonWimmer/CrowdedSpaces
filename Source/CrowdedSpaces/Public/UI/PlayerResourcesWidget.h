#pragma once

#include "CoreMinimal.h"
#include "CustomWidget.h"
#include "PlayerResourcesWidget.generated.h"

UCLASS()
class CROWDEDSPACES_API UPlayerResourcesWidget : public UCustomWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	virtual void Init_Implementation() override;
	virtual void Reset_Implementation() override;
};
