#include "AI/NPC.h"

#include "UI/Widgets/FoodBarWidget.h"

ANPC::ANPC()
{
	// Food
	FoodComponent = CreateDefaultSubobject<UFoodComponent>(TEXT("FoodComponent"));
	FoodBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FoodBarWidget"));
	FoodBarWidget->SetupAttachment(GetMesh());

	// Oxygen
	OxygenComponent = CreateDefaultSubobject<UOxygenComponent>(TEXT("OxygenComponent"));
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
	
	StartRemoveFood();
}

void ANPC::RemoveFood() const
{
	FoodComponent->RemoveFood(RemoveFoodPerInterval);
}

void ANPC::StartRemoveFood()
{
	GetWorldTimerManager().SetTimer(
		RemoveFoodTimerHandle,
		this,
		&ANPC::RemoveFood,
		RemoveFoodInterval,
		true // looping
	);
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
#pragma endregion Selectable

