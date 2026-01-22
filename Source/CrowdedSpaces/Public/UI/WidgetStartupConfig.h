#pragma once

#include "CoreMinimal.h"
#include "CustomWidget.h"
#include "WidgetStartupConfig.generated.h"

USTRUCT(BlueprintType)
struct FWidgetStartupConfig
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCustomWidget> WidgetClass;

	UPROPERTY(EditAnywhere)
	ESlateVisibility InitialVisibility = ESlateVisibility::Hidden;
};
