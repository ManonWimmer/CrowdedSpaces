#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectionWidget.generated.h"

UCLASS()
class CROWDEDSPACES_API USelectionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void Init();

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void Reset();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateSelection(const FString& DisplayName, const TMap<FString, FString>& Stats);
};
