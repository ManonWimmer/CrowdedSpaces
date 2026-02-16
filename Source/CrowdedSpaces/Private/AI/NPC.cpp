#include "AI/NPC.h"

#include "UI/Widgets/FoodBarWidget.h"

ANPC::ANPC()
{
	// Food
	FoodComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("FoodComponent"));
	FoodComponent->SetType(EResourceType::Food);
	FoodComponent->SetCanLoseAndRegenResource(true);
	
	FoodBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FoodBarWidget"));
	FoodBarWidget->SetupAttachment(GetMesh());

	// Oxygen
	OxygenComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("OxygenComponent"));
	OxygenComponent->SetType(EResourceType::Oxygen);
	OxygenComponent->SetCanLoseAndRegenResource(false);

	// Energy
	EnergyComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("EnergyComponent"));
	EnergyComponent->SetType(EResourceType::Energy);
	EnergyComponent->SetCanLoseAndRegenResource(true);

	// Selectable
	SelectionType = ESelectionType::NPC;
}

void ANPC::SetCurrentAction(ENPCAction NewAction)
{
	CurrentAction = NewAction;
	FString ActionString = StaticEnum<ENPCAction>()->GetDisplayNameTextByValue(static_cast<int64>(CurrentAction)).ToString();
	OnCurrentActionChanged.Broadcast(CurrentAction);
}

void ANPC::Die()
{
	// todo: animation ?

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Npc died.");
	
	Destroy();
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
	
	FoodComponent->OnNoMoreResource.AddDynamic(this, &ANPC::Die);
	EnergyComponent->OnNoMoreResource.AddDynamic(this, &ANPC::Die);
}

void ANPC::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FoodComponent->OnNoMoreResource.RemoveDynamic(this, &ANPC::Die);
	EnergyComponent->OnNoMoreResource.RemoveDynamic(this, &ANPC::Die);
	
	Super::EndPlay(EndPlayReason);
}

void ANPC::RemoveFood() const
{
	FoodComponent->RemoveResource(RemoveFoodPerInterval);
}

#pragma region Selectable
void ANPC::OnSelected()
{
}

void ANPC::OnDeselected()
{
}
#pragma endregion

