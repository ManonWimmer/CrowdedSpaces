#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameHUD.generated.h"

UCLASS()
class CROWDEDSPACES_API AGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void CreateAndInitBuildWidget();
	
private:
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UUserWidget> BuildWidgetBP;

	UPROPERTY()
	UUserWidget* BuildWidget;
};
