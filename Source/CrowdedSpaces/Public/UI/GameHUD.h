#pragma once

#include "CoreMinimal.h"
#include "BuildWidget.h"
#include "PlayerResourcesWidget.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

UCLASS()
class CROWDEDSPACES_API AGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// Build
	UFUNCTION()
	void CreateAndInitBuildWidget();
	
	UFUNCTION(BlueprintCallable, Category="Widgets")
	void ShowBuildWidget(bool bShow);

	// Player Resources (money etc)
	UFUNCTION()
	void CreateAndInitPlayerResourcesWidget();
	
	UFUNCTION(BlueprintCallable, Category="Widgets")
	void ShowPlayerResourcesWidget(bool bShow);
	
private:
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	// Build
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UUserWidget> BuildWidgetBP;

	UPROPERTY()
	UBuildWidget* BuildWidget;

	// Player Resources (money etc)
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UUserWidget> PlayerResourcesWidgetBP;

	UPROPERTY()
	UPlayerResourcesWidget* PlayerResourcesWidget;
};
