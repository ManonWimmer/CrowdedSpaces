#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildWidget.generated.h"

UCLASS()
class CROWDEDSPACES_API UBuildWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;
	
public:
	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void Init();
};
