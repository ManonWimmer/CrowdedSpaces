#include "UI/GameHUD.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	CreateAndInitPlayerResourcesWidget();
	CreateAndInitBuildWidget();
	CreateAndInitSelectionWidget();
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
		BuildWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
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
	ShowPlayerResourcesWidget(true); // shown at start 
}

void AGameHUD::ShowPlayerResourcesWidget(bool bShow)
{
	if (!PlayerResourcesWidget) return;
	
	if (bShow)
	{
		PlayerResourcesWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		PlayerResourcesWidget->Reset();
	}
	else
	{
		PlayerResourcesWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}
#pragma endregion Player Resources

#pragma region Selection
void AGameHUD::CreateAndInitSelectionWidget()
{
	if (!SelectionWidgetBP || !PlayerController) return;

	SelectionWidget = Cast<USelectionWidget>(CreateWidget(PlayerController, SelectionWidgetBP));
	
	SelectionWidget->AddToViewport(0);
	ShowPlayerResourcesWidget(false); 
}

void AGameHUD::ShowSelectionWidget(bool bShow)
{
	if (!SelectionWidget) return;
	
	if (bShow)
	{
		SelectionWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		SelectionWidget->Reset();
	}
	else
	{
		SelectionWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AGameHUD::UpdateSelectionWidget(const FString DisplayName, const TMap<FString, FString> Stats)
{
	if (!SelectionWidget) return;

	SelectionWidget->UpdateSelection(DisplayName, Stats);
}
#pragma endregion Selection
