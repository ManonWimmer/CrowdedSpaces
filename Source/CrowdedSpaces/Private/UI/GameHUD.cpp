#include "UI/GameHUD.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	CreateAndInitBuildWidget();
}

void AGameHUD::CreateAndInitBuildWidget()
{
	if (!BuildWidgetBP || !PlayerController) return;

	BuildWidget= CreateWidget(PlayerController, BuildWidgetBP);
	
	// plus tard : ui manager qui add to viewport en le gardant en ref pour potentiellement le hide / show
	BuildWidget->AddToViewport(0);
}
