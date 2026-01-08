#pragma once

#include "CoreMinimal.h"
#include "BuildWidget.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

UCLASS()
class CROWDEDSPACES_API AGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void CreateAndInitBuildWidget();

	// Build
	UFUNCTION(BlueprintCallable, Category="Widgets")
	void ShowBuildWidget(bool bShow);
	
private:
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	// Build
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UUserWidget> BuildWidgetBP;

	UPROPERTY()
	UBuildWidget* BuildWidget;
};
