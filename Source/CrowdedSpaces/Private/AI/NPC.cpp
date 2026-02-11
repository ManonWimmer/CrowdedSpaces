#include "AI/NPC.h"

#include "UI/Widgets/FoodBarWidget.h"

ANPC::ANPC()
{
	// Food
	FoodComponent = CreateDefaultSubobject<UFoodComponent>(TEXT("FoodComponent"));
	FoodComponent->SetSufferHunger(true);
	FoodBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FoodBarWidget"));
	FoodBarWidget->SetupAttachment(GetMesh());

	// Oxygen
	OxygenComponent = CreateDefaultSubobject<UOxygenComponent>(TEXT("OxygenComponent"));

	// Energy
	EnergyComponent = CreateDefaultSubobject<UEnergyComponent>(TEXT("EnergyComponent"));

	// Selectable
	SelectionType = ESelectionType::NPC;
}

void ANPC::SetCurrentAction(ENPCAction NewAction)
{
	CurrentAction = NewAction;
	FString ActionString = StaticEnum<ENPCAction>()->GetDisplayNameTextByValue(static_cast<int64>(CurrentAction)).ToString();
	OnCurrentActionChanged.Broadcast(CurrentAction);
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();

	// Cast food bar widget class to food bar widget -> set owning actor
	if (!FoodBarWidget)
		return;

	TObjectPtr<UUserWidget> UserWidget = FoodBarWidget->GetUserWidgetObject();
	if (!UserWidget)
		return;

	TObjectPtr<UFoodBarWidget> FoodWidget = Cast<UFoodBarWidget>(UserWidget);
	if (!FoodWidget)
		return;
	
	FoodWidget->OwningActor = this;
	FoodWidget->Init();
}

void ANPC::RemoveFood() const
{
	FoodComponent->RemoveFood(RemoveFoodPerInterval);
}

#pragma region Selectable
void ANPC::OnSelected()
{
}

void ANPC::OnDeselected()
{
}
#pragma endregion

