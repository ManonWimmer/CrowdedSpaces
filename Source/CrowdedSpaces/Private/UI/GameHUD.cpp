#include "UI/GameHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	CreateAndInitBuildWidget();
	CreateAndInitPlayerResourcesWidget();
}

#pragma region Build
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
#pragma endregion Build

#pragma region Player Resources
void AGameHUD::CreateAndInitPlayerResourcesWidget()
{
	if (!PlayerResourcesWidgetBP || !PlayerController) return;

	PlayerResourcesWidget = Cast<UPlayerResourcesWidget>(CreateWidget(PlayerController, PlayerResourcesWidgetBP));
	
	PlayerResourcesWidget->AddToViewport(0);
	ShowPlayerResourcesWidget(true); // shown at start but no clicks
}

void AGameHUD::ShowPlayerResourcesWidget(bool bShow)
{
	if (!PlayerResourcesWidget) return;
	
	if (bShow)
	{
		PlayerResourcesWidget->SetVisibility(ESlateVisibility::HitTestInvisible);
		PlayerResourcesWidget->Reset();
	}
	else
	{
		PlayerResourcesWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
#pragma endregion Player Resources