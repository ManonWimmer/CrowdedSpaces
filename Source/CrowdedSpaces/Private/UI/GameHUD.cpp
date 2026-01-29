#include "UI/GameHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widgets/MoralEventWidget.h"
#include "UI/CustomWidget.h"
#include "UI/Widgets/GeneratorSelectionWidget.h"
#include "UI/Widgets/SelectionWidget.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = GetOwningPlayerController();
	if (!PlayerController)
		return;
	
	// Create widgets
	CreateStartupWidgets();
}

void AGameHUD::CreateStartupWidgets()
{
	for (const auto& [WidgetClass, InitialVisibility] : StartupWidgetsConfig)
	{
		if (!WidgetClass) continue;

		UCustomWidget* Widget = CreateWidget<UCustomWidget>(PlayerController, WidgetClass);

		if (!Widget) continue;

		Widget->AddToViewport();
		Widget->SetVisibility(InitialVisibility);
		Widget->Init();

		WidgetInstances.Add(WidgetClass, Widget);
	}
}

#pragma region Build
void AGameHUD::ShowBuildWidget(bool bShow)
{
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
	LastSelectionWidget = ShowWidget(SelectionWidgetBP, bShow, ESlateVisibility::SelfHitTestInvisible);
	LastSelectionWidget = GetOrCreateWidget<USelectionWidget>(SelectionWidgetBP);
}

void AGameHUD::HideLastSelectionWidget()
{
	if (!LastSelectionWidget)
		return;
	
	LastSelectionWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AGameHUD::ShowGeneratorSelectionWidget(bool bShow)
{
	ShowWidget(GeneratorSelectionWidgetBP, bShow, ESlateVisibility::SelfHitTestInvisible);
	LastSelectionWidget = GetOrCreateWidget<UGeneratorSelectionWidget>(GeneratorSelectionWidgetBP);
}

void AGameHUD::UpdateSelectionWidget(const FString DisplayName, const TMap<FString, FString> Stats)
{
	//auto SelectionWidget = GetOrCreateWidget<USelectionWidget>(SelectionWidgetBP);
	//auto SelectionWidget = GetOrCreateWidget<UGeneratorSelectionWidget>(GeneratorSelectionWidgetBP); // test ici aussi
	
	if (!LastSelectionWidget)
		return;

	LastSelectionWidget->UpdateSelection(DisplayName, Stats);
}
#pragma endregion Selection

#pragma region Moral Event
void AGameHUD::ShowMoralEventWidget(bool bShow)
{
	ShowWidget(MoralEventWidgetBP, bShow, ESlateVisibility::SelfHitTestInvisible);
}

void AGameHUD::UpdateMoralEventWidget(const UMoralEvent* MoralEvent)
{
	auto MoralEventWidget = GetOrCreateWidget<UMoralEventWidget>(MoralEventWidgetBP);
	if (!MoralEventWidget)
		return;

	MoralEventWidget->Update(MoralEvent);
}
#pragma endregion Moral Event

#pragma region Generic Functions
UCustomWidget* AGameHUD::ShowWidget(TSubclassOf<UCustomWidget> WidgetClass, bool bShow, ESlateVisibility VisibilityOnShow)
{
	if (!WidgetClass)
		return nullptr;

	UCustomWidget* Widget = GetOrCreateWidget<UCustomWidget>(WidgetClass);
	if (!Widget)
		return nullptr;

	if (bShow)
	{
		Widget->SetVisibility(VisibilityOnShow);
		Widget->Reset(); 
	}
	else
	{
		Widget->SetVisibility(ESlateVisibility::Hidden);
	}

	return Widget;
}


template <typename T>
T* AGameHUD::GetOrCreateWidget(TSubclassOf<UCustomWidget> WidgetClass)
{
	if (!WidgetClass || !PlayerController)
		return nullptr;

	if (UCustomWidget** Found = WidgetInstances.Find(WidgetClass))
	{
		return Cast<T>(*Found); 
	}

	UCustomWidget* NewWidget = CreateWidget<UCustomWidget>(PlayerController, WidgetClass);

	if (!NewWidget)
		return nullptr;

	NewWidget->AddToViewport();
	NewWidget->SetVisibility(ESlateVisibility::Hidden);
	NewWidget->Init();

	WidgetInstances.Add(WidgetClass, NewWidget);

	return Cast<T>(NewWidget);
}
#pragma endregion Generic Functions