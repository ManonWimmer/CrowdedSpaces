#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerResourcesWidget.generated.h"

UCLASS()
class CROWDEDSPACES_API UPlayerResourcesWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void Init();

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void Reset();
};
