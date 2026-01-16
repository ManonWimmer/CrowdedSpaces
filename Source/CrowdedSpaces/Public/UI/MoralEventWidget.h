#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MoralEventWidget.generated.h"

UCLASS()
class CROWDEDSPACES_API UMoralEventWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void Init();

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void Reset();

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void Update();
};
