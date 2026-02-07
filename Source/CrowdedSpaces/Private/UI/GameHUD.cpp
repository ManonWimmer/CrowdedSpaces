#include "UI/GameHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widgets/MoralEventWidget.h"
#include "UI/CustomWidget.h"
#include "UI/Widgets/SelectionWidget.h"
#include "UI/Widgets/Selection/RoomSelectionWidget.h"

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

		TObjectPtr<UCustomWidget> Widget = CreateWidget<UCustomWidget>(PlayerController, WidgetClass);

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
void AGameHUD::ShowSelectionWidget(AActor* SelectableActor, bool bShow, ESelectionType SelectionType)
{
	if (!bShow || !SelectableActor)
	{
		HideCurrentSelectionWidget();
		return;
	}

	// Toggle
	if (CurrentlySelectedActor == SelectableActor)
	{
		HideCurrentSelectionWidget();
		return;
	}

	// Hide previous
	HideCurrentSelectionWidget();

	UCustomWidget* Widget = GetWidgetFromSelectionType(SelectionType);
	if (!Widget)
		return;

	Widget->Setup(SelectableActor);

	CurrentlySelectedActor = SelectableActor;
}

void AGameHUD::ShowSelectionWidget(FGridRoom& Room, bool bShow, ESelectionType SelectionType)
{
	if (!bShow)
	{
		HideCurrentSelectionWidget();
		return;
	}

	// Toggle ONLY if same room
	if (CurrentlySelectedRoom == &Room)
	{
		HideCurrentSelectionWidget();
		return;
	}

	UCustomWidget* Widget = GetWidgetFromSelectionType(SelectionType);
	if (!Widget)
		return;

	URoomSelectionWidget* RoomWidget = Cast<URoomSelectionWidget>(Widget);
	if (!RoomWidget)
		return;

	RoomWidget->SetupRoom(Room);

	CurrentlySelectedRoom = &Room;
	CurrentlySelectedActor = nullptr;
}

void AGameHUD::HideCurrentSelectionWidget()
{
	if (!CurrentlyShownSelectionWidgetBP)
		return;

	UCustomWidget* Widget = ShowWidget(CurrentlyShownSelectionWidgetBP, false, ESlateVisibility::SelfHitTestInvisible);

	if (Widget)
		Widget->Unsetup();

	CurrentlyShownSelectionWidgetBP = nullptr;
	CurrentlySelectedActor = nullptr;
	CurrentlySelectedRoom = nullptr;
}

UCustomWidget* AGameHUD::GetWidgetFromSelectionType(ESelectionType Type)
{
	switch (Type)
	{
	case ESelectionType::NPC:
		CurrentlyShownSelectionWidgetBP = NPCSelectionWidgetBP;
		break;

	case ESelectionType::Generator:
		CurrentlyShownSelectionWidgetBP = GeneratorSelectionWidgetBP;
		break;

	case ESelectionType::Room:
		CurrentlyShownSelectionWidgetBP = RoomSelectionWidgetBP;
		break;

	case ESelectionType::Food:
		CurrentlyShownSelectionWidgetBP = FoodSelectionWidgetBP;
		break;
		
	case ESelectionType::Bed:
		CurrentlyShownSelectionWidgetBP = BedSelectionWidgetBP;
		break;

	default:
		return nullptr;
	}

	return ShowWidget(CurrentlyShownSelectionWidgetBP, true, ESlateVisibility::SelfHitTestInvisible);
}
#pragma endregion Selection

#pragma region Moral Event
void AGameHUD::ShowMoralEventWidget(bool bShow)
{
	ShowWidget(MoralEventWidgetBP, bShow, ESlateVisibility::Visible); // Impossible de cliquer sur boutons pendant (build, time etc)
}

void AGameHUD::UpdateMoralEventWidget(const UMoralEvent* MoralEvent)
{
	TObjectPtr<UMoralEventWidget> MoralEventWidget = GetOrCreateWidget<UMoralEventWidget>(MoralEventWidgetBP);
	if (!MoralEventWidget)
		return;

	MoralEventWidget->Update(MoralEvent);
}
#pragma endregion Moral Event

#pragma region Time
void AGameHUD::ShowTimeWidget(bool bShow)
{
	ShowWidget(TimeBP, bShow, ESlateVisibility::SelfHitTestInvisible);
}
#pragma endregion Time

#pragma region Generic Functions
UCustomWidget* AGameHUD::ShowWidget(TSubclassOf<UCustomWidget> WidgetClass, bool bShow, ESlateVisibility VisibilityOnShow)
{
	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Log, TEXT("return !widgetclass"));
		return nullptr;
	}

	TObjectPtr<UCustomWidget> Widget = GetOrCreateWidget<UCustomWidget>(WidgetClass);
	if (!Widget)
	{
		UE_LOG(LogTemp, Log, TEXT("return !widget"));
		return nullptr;
	}

	if (bShow)
	{
		UE_LOG(LogTemp, Log, TEXT("bshow"));
		Widget->SetVisibility(VisibilityOnShow);
		Widget->Reset();
		return Widget;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("bshow false"));
		Widget->SetVisibility(ESlateVisibility::Hidden);
		return Widget;
	}
}


template <typename T>
TObjectPtr<T> AGameHUD::GetOrCreateWidget(TSubclassOf<UCustomWidget> WidgetClass)
{
	if (!WidgetClass || !PlayerController)
		return nullptr;

	if (TObjectPtr<UCustomWidget>* Found = WidgetInstances.Find(WidgetClass))
	{
		return Cast<T>(*Found); 
	}

	TObjectPtr<UCustomWidget> NewWidget = CreateWidget<UCustomWidget>(PlayerController, WidgetClass);

	if (!NewWidget)
		return nullptr;

	NewWidget->AddToViewport();
	NewWidget->SetVisibility(ESlateVisibility::Hidden);
	NewWidget->Init();

	WidgetInstances.Add(WidgetClass, NewWidget);

	return Cast<T>(NewWidget);
}
#pragma endregion Generic Functions