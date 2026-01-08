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

	BuildWidget = Cast<UBuildWidget>(CreateWidget(PlayerController, BuildWidgetBP));
	
	BuildWidget->AddToViewport(0);
	ShowBuildWidget(false); // hidden at start
}

void AGameHUD::ShowBuildWidget(bool bShow)
{
	if (!BuildWidget) return;
	
	if (bShow)
	{
		BuildWidget->SetVisibility(ESlateVisibility::Visible);
		BuildWidget->Reset();
	}
	else
	{
		BuildWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
