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
}

void ANPC::SetCurrentAction(ENPCAction NewAction)
{
	CurrentAction = NewAction;
	FString ActionString = StaticEnum<ENPCAction>()->GetDisplayNameTextByValue(static_cast<int64>(CurrentAction)).ToString();
	OnStatChanged.Broadcast("Current Action", ActionString);
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();

	// Cast food bar widget class to food bar widget -> set owning actor
	if (!FoodBarWidget)
		return;

	UUserWidget* UserWidget = FoodBarWidget->GetUserWidgetObject();
	if (!UserWidget)
		return;

	UFoodBarWidget* FoodWidget = Cast<UFoodBarWidget>(UserWidget);
	if (!FoodWidget)
		return;
	
	FoodWidget->OwningActor = this;
	FoodWidget->Init();
	
	//StartRemoveFood(); now managed in food component directly if suffer hunger
}

void ANPC::RemoveFood() const
{
	FoodComponent->RemoveFood(RemoveFoodPerInterval);
}

#pragma region Selectable
void ANPC::OnSelected()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "on selected ");
}

void ANPC::OnDeselected()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "on deselected ");
}

FString ANPC::GetDisplayName() const
{
	return "NPC";
}

AActor* ANPC::GetSelectableActor()
{
	return this;
}

TArray<FStat> ANPC::GetCurrentValues() const
{
	TArray<TPair<FString, FString>> Values;
	FString ActionString = StaticEnum<ENPCAction>()->GetDisplayNameTextByValue(static_cast<int64>(CurrentAction)).ToString();
	Values.Add(TPair<FString, FString>(FString("Current Action"), ActionString));
	return Values;
}
#pragma endregion Selectable

