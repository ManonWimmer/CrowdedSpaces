#include "UI/GameHUD.h"
#include "Blueprint/UserWidget.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = GetOwningPlayerController();
	if (!PlayerController) return;
	
	// Create widgets
	CreateStartupWidgets();
}

void AGameHUD::CreateStartupWidgets()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, "Create startup widgets");
	for (const FWidgetStartupConfig& Config : StartupWidgetsConfig)
	{
		if (!Config.WidgetClass) continue;

		UCustomWidget* Widget = CreateWidget<UCustomWidget>(PlayerController, Config.WidgetClass);

		if (!Widget) continue;

		Widget->AddToViewport();
		Widget->SetVisibility(Config.InitialVisibility);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, "Init from startup widgets");
		Widget->Init();

		WidgetInstances.Add(Config.WidgetClass, Widget);
	}
}

#pragma region Build
void AGameHUD::ShowBuildWidget(bool bShow)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, "Show build widget");
	ShowWidget(BuildWidgetBP, bShow, ESlateVisibility::SelfHitTestInvisible);
}
#pragma endregion Build

#pragma region Player Resources
void AGameHUD::ShowPlayerResourcesWidget(bool bShow)
{
	ShowWidget(PlayerResourcesWidgetBP, bShow, ESlateVisibility::SelfHitTestInvisible);
}
#pragma endregion Player Resources

#pragma region Selection
void AGameHUD::ShowSelectionWidget(bool bShow)
{
	ShowWidget(SelectionWidgetBP, bShow, ESlateVisibility::SelfHitTestInvisible);
}

void AGameHUD::UpdateSelectionWidget(const FString DisplayName, const TMap<FString, FString> Stats)
{
	if (!SelectionWidget) return;

	SelectionWidget->UpdateSelection(DisplayName, Stats);
}
#pragma endregion Selection

#pragma region Moral Event
void AGameHUD::ShowMoralEventWidget(bool bShow)
{
	ShowWidget(MoralEventWidgetBP, bShow, ESlateVisibility::SelfHitTestInvisible);
}

void AGameHUD::UpdateMoralEventWidget(const UMoralEventData* EventData)
{
	/*
	if (!MoralEventWidget) return;

	MoralEventWidget->Update(EventData);
	*/
}
#pragma endregion Moral Event

#pragma region Generic Functions
void AGameHUD::ShowWidget(TSubclassOf<UCustomWidget> WidgetClass, bool bShow, ESlateVisibility VisibilityOnShow)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, "Show widget");
	if (!WidgetClass) return;

	UCustomWidget* Widget = GetOrCreateWidget<UCustomWidget>(WidgetClass);
	if (!Widget) return;

	if (bShow)
	{
		Widget->SetVisibility(VisibilityOnShow);
		Widget->Reset(); 
	}
	else
	{
		Widget->SetVisibility(ESlateVisibility::Hidden);
	}
}


template <typename T>
T* AGameHUD::GetOrCreateWidget(TSubclassOf<UCustomWidget> WidgetClass)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, "Get or create widget");
	
	if (!WidgetClass || !PlayerController) return nullptr;

	if (UCustomWidget** Found = WidgetInstances.Find(WidgetClass))
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, "Get found widget");
		return Cast<T>(*Found); 
	}

	UCustomWidget* NewWidget = CreateWidget<UCustomWidget>(PlayerController, WidgetClass);

	if (!NewWidget) return nullptr;

	NewWidget->AddToViewport();
	NewWidget->SetVisibility(ESlateVisibility::Hidden);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Blue, "Init from get or create widget");
	NewWidget->Init();

	WidgetInstances.Add(WidgetClass, NewWidget);

	return Cast<T>(NewWidget);
}
#pragma endregion Generic Functions
