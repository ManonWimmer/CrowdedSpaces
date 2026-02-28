#include "AI/NPC.h"

#include "Camera/FreeCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/Widgets/FoodBarWidget.h"
#include "UI/Widgets/NPCActionWidget.h"

ANPC::ANPC()
{
	// Food
	FoodComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("FoodComponent"));
	FoodComponent->SetType(EResourceType::Food);
	FoodComponent->SetCanLoseAndRegenResource(true);
	
	FoodBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FoodBarWidget"));
	FoodBarWidget->SetupAttachment(GetMesh());

	NPCActionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NPCActionWidget"));
	NPCActionWidget->SetupAttachment(GetMesh());

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

void ANPC::SetCurrentAction(ENPCActionWidget NewAction)
{
	CurrentAction = NewAction;
	FString ActionString = StaticEnum<ENPCActionWidget>()->GetDisplayNameTextByValue(static_cast<int64>(CurrentAction)).ToString();
	
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

	TObjectPtr<UUserWidget> FoodBarUserWidget = FoodBarWidget->GetUserWidgetObject();
	if (!FoodBarUserWidget)
		return;

	TObjectPtr<UFoodBarWidget> FoodBarWidgetPtr = Cast<UFoodBarWidget>(FoodBarUserWidget);
	if (!FoodBarWidgetPtr)
		return;
	
	FoodBarWidgetPtr->OwningActor = this;
	FoodBarWidgetPtr->Init();

	// Same for npc action widget
	if (!NPCActionWidget)
		return;

	TObjectPtr<UUserWidget> NPCActionUserWidget = NPCActionWidget->GetUserWidgetObject();
	if (!NPCActionUserWidget)
		return;

	TObjectPtr<UNPCActionWidget> NPCActionWidgetPtr = Cast<UNPCActionWidget>(NPCActionUserWidget);
	if (!NPCActionWidgetPtr)
		return;
	
	NPCActionWidgetPtr->OwningActor = this;
	NPCActionWidgetPtr->Init();

	// Die
	FoodComponent->OnNoMoreResource.AddDynamic(this, &ANPC::Die);
	EnergyComponent->OnNoMoreResource.AddDynamic(this, &ANPC::Die);
}

void ANPC::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FoodComponent->OnNoMoreResource.RemoveDynamic(this, &ANPC::Die);
	EnergyComponent->OnNoMoreResource.RemoveDynamic(this, &ANPC::Die);
	
	Super::EndPlay(EndPlayReason);
}

void ANPC::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Widgets world look at camera
	if (!FoodBarWidget || !NPCActionWidget) return;

	const APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	const AFreeCameraPawn* CamPawn = Cast<AFreeCameraPawn>(PC->GetPawn());
	if (!CamPawn) return;

	const UCameraComponent* Cam = CamPawn->GetCameraComponent();
	if (!Cam) return;

	const FVector CameraLocation = Cam->GetComponentLocation();
	const FVector FoodBarWidgetLocation = FoodBarWidget->GetComponentLocation();
	const FVector NPCActionWidgetLocation = NPCActionWidget->GetComponentLocation();

	const FRotator LookAtFoodBar = UKismetMathLibrary::FindLookAtRotation(FoodBarWidgetLocation, CameraLocation);
	const FRotator LookAtNPCAction = UKismetMathLibrary::FindLookAtRotation(NPCActionWidgetLocation, CameraLocation);
	
	const FRotator YawOnlyFoodBar(0.f, LookAtFoodBar.Yaw, 0.f);
	const FRotator YawOnlyNPCAction(0.f, LookAtFoodBar.Yaw, 0.f);

	FoodBarWidget->SetWorldRotation(YawOnlyFoodBar);
	NPCActionWidget->SetWorldRotation(YawOnlyNPCAction);
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

