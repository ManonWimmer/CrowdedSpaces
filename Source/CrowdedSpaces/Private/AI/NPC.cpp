#include "AI/NPC.h"

#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Build/BuildableObject.h"
#include "Camera/FreeCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "Game/CrowdedGameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/Widgets/FoodBarWidget.h"
#include "UI/Widgets/NPCActionWidget.h"

ANPC::ANPC()
{
	// Food
	FoodComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("FoodComponent"));
	FoodComponent->SetType(EResourceType::Food);
	FoodComponent->SetCanLoseAndRegenResource(true);
	ResourceMap.Add(EResourceType::Food, FoodComponent);
	
	FoodBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FoodBarWidget"));
	FoodBarWidget->SetupAttachment(GetMesh());

	NPCActionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NPCActionWidget"));
	NPCActionWidget->SetupAttachment(GetMesh());

	// Energy
	EnergyComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("EnergyComponent"));
	EnergyComponent->SetType(EResourceType::Energy);
	EnergyComponent->SetCanLoseAndRegenResource(true);
	ResourceMap.Add(EResourceType::Energy, EnergyComponent);

	// Selectable
	SelectionType = ESelectionType::NPC;
}

UResourceComponent* ANPC::GetResourceComponentByType(const EResourceType Type) const
{
	if (const TObjectPtr<UResourceComponent>* Found = ResourceMap.Find(Type))
	{
		return Found->Get();
	}

	return nullptr;
}

int ANPC::GetResourceByType(const EResourceType Type) const
{
	if (!GetResourceComponentByType(Type))
		return 0;
	else
		return GetResourceComponentByType(Type)->GetResource();
}

void ANPC::SetCurrentAction(const ENPCActionWidget NewAction)
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

	ACrowdedGameMode* GameMode = GetWorld()->GetAuthGameMode<ACrowdedGameMode>();
	if (!GameMode)
		return;

	GameMode->UnregisterNPC(this);
	
	Destroy();
}

void ANPC::SetWorkOnGeneratorType(const EProductionType NewType)
{
	if (WorkOnGeneratorType == NewType)
		return;
	
	WorkOnGeneratorType = NewType;

	// Cancel use generator task in cas was working on genrator with diferent type
	if (ANPCController* ControllerNPC = Cast<ANPCController>(GetController()))
	{
		if (UBlackboardComponent* BB = ControllerNPC->GetBlackboardComponent())
		{
			TObjectPtr<ABuildableObject> TargetObject = Cast<ABuildableObject>(BB->GetValueAsObject("TargetObject"));
			
			if (TargetObject && TargetObject->GetObjectType() == EObjectType::Generator)
			{
				if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(ControllerNPC->GetBrainComponent()))
					BTComp->RestartTree();
			}
		}
	}
}

int ANPC::GetProductionMultiplierForType(const EProductionType Type) const
{
	switch (Type)
	{
		case EProductionType::Money:
			return MoneyProductionMultiplier;
			
		case EProductionType::Food:
			return FoodProductionMultiplier;
			
		case EProductionType::Electricity:
			return ElectricityProductionMultiplier;
			
		default:
			return 1;
	}
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

	ACrowdedGameMode* GameMode = GetWorld()->GetAuthGameMode<ACrowdedGameMode>();
	if (!GameMode)
		return;

	GameMode->RegisterNPC(this);

	// Multipliers
	FoodProductionMultiplier = FMath::RandRange(1, 5);
	ElectricityProductionMultiplier = FMath::RandRange(1, 5);
	MoneyProductionMultiplier = FMath::RandRange(1, 5);
}

void ANPC::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FoodComponent->OnNoMoreResource.RemoveDynamic(this, &ANPC::Die);
	EnergyComponent->OnNoMoreResource.RemoveDynamic(this, &ANPC::Die);
	
	Super::EndPlay(EndPlayReason);
}

void ANPC::Tick(const float DeltaSeconds)
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

