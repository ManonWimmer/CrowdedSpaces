#pragma once

#include "CoreMinimal.h"
#include "UI/CustomWidget.h"
#include "TimeWidget.generated.h"

UCLASS()
class CROWDEDSPACES_API UTimeWidget : public UCustomWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	virtual void Init_Implementation() override;
	virtual void Reset_Implementation() override;
};
