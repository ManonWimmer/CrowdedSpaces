#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CustomWidget.generated.h"

class AGameHUD;

UCLASS(Abstract) // C'est ça qui manquait !!
class CROWDEDSPACES_API UCustomWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Widget")
	void Init();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Widget")
	void Reset();

protected:
	virtual void Init_Implementation();
	virtual void Reset_Implementation();

	UPROPERTY()
	TObjectPtr<AGameHUD> GameHUD = nullptr;
};
