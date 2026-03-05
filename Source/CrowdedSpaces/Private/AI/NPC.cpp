#include "AI/NPC.h"

#include "AI/NameGeneratorSubsystem.h"
#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Build/BuildableObject.h"
#include "Camera/FreeCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "Game/CrowdedGameMode.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/Widgets/FoodBarWidget.h"
#include "UI/Widgets/NPCActionWidget.h"
#include "UI/Widgets/Selection/NPCNameWidget.h"

ANPC::ANPC()
{
	// Food
	FoodComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("FoodComponent"));
	FoodComponent->SetType(EResourceType::Food);
	FoodComponent->SetCanLoseAndRegenResource(true);
	ResourceMap.Add(EResourceType::Food, FoodComponent);
	
	NPCNameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NPCNameWidget"));
	NPCNameWidget->SetupAttachment(GetMesh());

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

void ANPC::TryGenerateName()
{
	if (const UNameGeneratorSubsystem* NameSystem =
	   GetWorld()->GetSubsystem<UNameGeneratorSubsystem>())
	{
		if (NameSystem->IsInitialized())
		{
			NPCName = NameSystem->GenerateName();
			
			// Setup npc name widget
			if (!NPCNameWidget)
				return;

			const TObjectPtr<UUserWidget> NPCNameUserWidget = NPCNameWidget->GetUserWidgetObject();
			if (!NPCNameUserWidget)
				return;

			const TObjectPtr<UNPCNameWidget> NPCNameWidgetPtr = Cast<UNPCNameWidget>(NPCNameUserWidget);
			if (!NPCNameWidgetPtr)
				return;
	
			NPCNameWidgetPtr->OwningActor = this;
			NPCNameWidgetPtr->Init();
			
			return;
		}
	}

	// Retry dans 0.5s
	GetWorld()->GetTimerManager().SetTimer(
		NameRetryTimer,
		this,
		&ANPC::TryGenerateName,
		0.5f,
		false
	);
}

FLinearColor ANPC::GetRandomColor()
{
	const float Hue = FMath::FRandRange(0.f, 1.f);
	const float Saturation = FMath::FRandRange(0.6f, 0.85f);
	const float Value = FMath::FRandRange(0.7f, 0.95f);

	const FLinearColor Color = FLinearColor::MakeFromHSV8(
		Hue * 255,
		Saturation * 255,
		Value * 255
	);

	return Color;
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
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

	// Random name
	TryGenerateName();

	// Random color
	UPrimitiveComponent* MeshComp = GetMesh();

	if (!MeshComp) return;

	BodyMaterialInstance = MeshComp->CreateAndSetMaterialInstanceDynamic(0);
	OtherMaterialInstance = MeshComp->CreateAndSetMaterialInstanceDynamic(1);

	const FLinearColor RandomColor = GetRandomColor();
	
	if (BodyMaterialInstance)
	{
		BodyMaterialInstance->SetVectorParameterValue(TEXT("BaseColor"), RandomColor);
	}

	if (OtherMaterialInstance)
	{
		OtherMaterialInstance->SetVectorParameterValue(TEXT("BaseColor"), RandomColor);
	}
	
	// Setup npc action widget
	if (!NPCActionWidget)
		return;

	const TObjectPtr<UUserWidget> NPCActionUserWidget = NPCActionWidget->GetUserWidgetObject();
	if (!NPCActionUserWidget)
		return;

	const TObjectPtr<UNPCActionWidget> NPCActionWidgetPtr = Cast<UNPCActionWidget>(NPCActionUserWidget);
	if (!NPCActionWidgetPtr)
		return;
	
	NPCActionWidgetPtr->OwningActor = this;
	NPCActionWidgetPtr->Init();
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
	if (!NPCNameWidget || !NPCActionWidget) return;

	const APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	const AFreeCameraPawn* CamPawn = Cast<AFreeCameraPawn>(PC->GetPawn());
	if (!CamPawn) return;

	const UCameraComponent* Cam = CamPawn->GetCameraComponent();
	if (!Cam) return;

	const FVector CameraLocation = Cam->GetComponentLocation();
	const FVector NPCNameWidgetLocation = NPCNameWidget->GetComponentLocation();
	const FVector NPCActionWidgetLocation = NPCActionWidget->GetComponentLocation();

	const FRotator LookAtNPCName = UKismetMathLibrary::FindLookAtRotation(NPCNameWidgetLocation, CameraLocation);
	const FRotator LookAtNPCAction = UKismetMathLibrary::FindLookAtRotation(NPCActionWidgetLocation, CameraLocation);
	
	const FRotator YawOnlyNPCName(0.f, LookAtNPCName.Yaw, 0.f);
	const FRotator YawOnlyNPCAction(0.f, LookAtNPCAction.Yaw, 0.f);

	NPCNameWidget->SetWorldRotation(YawOnlyNPCName);
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

