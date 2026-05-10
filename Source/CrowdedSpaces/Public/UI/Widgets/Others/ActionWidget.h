#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionWidget.generated.h"

class UAction;

UCLASS()
class CROWDEDSPACES_API UActionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetupActions(const TArray<UAction*>& Actions, AActor* Actor);
};
