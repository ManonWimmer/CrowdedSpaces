#include "AI/NPC.h"

#include "Action/Action.h"
#include "Action/ActionComponent.h"
#include "AI/NameGeneratorSubsystem.h"
#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Build/BuildableObject.h"
#include "Build/SlotComponent.h"
#include "Camera/FreeCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Game/CrowdedGameMode.h"
#include "Game/CrowdedGameState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Training/TrainingData.h"
#include "Training/TrainingSubsystem.h"
#include "UI/Widgets/FoodBarWidget.h"
#include "UI/Widgets/NPCActionWidget.h"
#include "UI/Widgets/Selection/NPCNameWidget.h"

#define BO_LOG(Format, ...) UE_LOG(LogTemp, Warning, TEXT("[BuildableObject:%s] " Format), *GetNameSafe(this), ##__VA_ARGS__)

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

	AIControllerClass = ANPCController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Actions
	ActionComponent = CreateDefaultSubobject<UActionComponent>(TEXT("ActionComponent"));

	// Portrait
	PortraitCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("PortraitCapture"));
	PortraitCapture->SetupAttachment(GetMesh());
	PortraitCapture->bCaptureEveryFrame = false;
	PortraitCapture->bCaptureOnMovement = false;
	PortraitCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
	PortraitCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
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
	FoodProductionMultiplier = 1;
	ElectricityProductionMultiplier = 1;
	MoneyProductionMultiplier = 1;

	// Training exp
	TrainingSubsystem = GetWorld()->GetSubsystem<UTrainingSubsystem>();
	
	TrainingSkillsExp.Add(ETrainingSkillType::MoneyProduction, 0);
	TrainingSkillsExp.Add(ETrainingSkillType::FoodProduction, 0);
	TrainingSkillsExp.Add(ETrainingSkillType::ElectricityProduction, 0);

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

	// Actions
	ACrowdedGameState* GameState = GetWorld()->GetGameState<ACrowdedGameState>();

	TArray<TObjectPtr<UAction>> InstancedActions;

	for (const TSubclassOf<UAction>& ActionClass : GameState->NPCActions)
	{
		if (!ActionClass) continue;

		UAction* NewAction = NewObject<UAction>(this, ActionClass);
		if (!NewAction) continue;

		NewAction->Initialize(GetWorld());
		InstancedActions.Add(NewAction);
	}

	ActionComponent->SetupActions(InstancedActions);

	// Portrait
	SetupCapture();
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

#pragma region Training
void ANPC::AddTrainingExp(const float AddExp)
{
	const float CurrentLevelExpNeeded = GetCurrentLevelNeededExp(TrainingSkillType);
	
	switch (TrainingSkillType)
	{
		case ETrainingSkillType::MoneyProduction:
			{
				if (MoneyProductionMultiplier == MaxMultipliersLevel) // Max
					break;
				
				TrainingSkillsExp[ETrainingSkillType::MoneyProduction] += AddExp;

				if (TrainingSkillsExp[ETrainingSkillType::MoneyProduction] > CurrentLevelExpNeeded)
				{
					TrainingSkillsExp[ETrainingSkillType::MoneyProduction] = 0;
					MoneyProductionMultiplier += 1;
				}
				break;
			}
			
		
		case ETrainingSkillType::FoodProduction:
			{
				if (FoodProductionMultiplier == MaxMultipliersLevel) 
					break;
				
				TrainingSkillsExp[ETrainingSkillType::FoodProduction] += AddExp;

				if (TrainingSkillsExp[ETrainingSkillType::FoodProduction] > CurrentLevelExpNeeded)
				{
					TrainingSkillsExp[ETrainingSkillType::FoodProduction] = 0;
					FoodProductionMultiplier += 1;
				}
				break;
			}
		
		case ETrainingSkillType::ElectricityProduction:
			{
				if (FoodProductionMultiplier == MaxMultipliersLevel) 
					break;
				
				TrainingSkillsExp[ETrainingSkillType::FoodProduction] += AddExp;

				if (TrainingSkillsExp[ETrainingSkillType::FoodProduction] > CurrentLevelExpNeeded)
				{
					TrainingSkillsExp[ETrainingSkillType::FoodProduction] = 0;
					FoodProductionMultiplier += 1;
				}
				break;
			}
	}

	OnSkillsTrained.Broadcast();
}

int ANPC::GetCurrentLevel(const ETrainingSkillType TrainingSkillTypeToUpdate) const
{
	int CurrentLevel = 0;
	
	switch (TrainingSkillTypeToUpdate)
	{
		case ETrainingSkillType::MoneyProduction:
			CurrentLevel = MoneyProductionMultiplier - 1;  
			break;
			
		case ETrainingSkillType::FoodProduction:
			CurrentLevel = FoodProductionMultiplier - 1;
			break;
			
		case ETrainingSkillType::ElectricityProduction:
			CurrentLevel = ElectricityProductionMultiplier - 1;
			break;
	}

	return CurrentLevel;
}

float ANPC::GetCurrentLevelExp(const ETrainingSkillType TrainingSkillTypeToUpdate) const
{
	switch (TrainingSkillTypeToUpdate)
	{
		case ETrainingSkillType::MoneyProduction:
			return TrainingSkillsExp[ETrainingSkillType::MoneyProduction];
		
		case ETrainingSkillType::FoodProduction:
			return TrainingSkillsExp[ETrainingSkillType::FoodProduction];
		
		case ETrainingSkillType::ElectricityProduction:
			return TrainingSkillsExp[ETrainingSkillType::ElectricityProduction];
	}

	return 0;
}

float ANPC::GetCurrentLevelNeededExp(const ETrainingSkillType TrainingSkillTypeToUpdate) const
{
	const int CurrentLevel = GetCurrentLevel(TrainingSkillTypeToUpdate);

	if (TrainingSubsystem->GetTrainingData()->TrainingLevelAndExpNeeded.Contains(CurrentLevel))
		return TrainingSubsystem->GetTrainingData()->TrainingLevelAndExpNeeded[CurrentLevel];

	else
		return 0;
}

#pragma endregion

#pragma region Death
void ANPC::Die()
{
	// todo: animation ?

	BO_LOG("Npc death, food : %f, energy : %f", FoodComponent->GetResource(), EnergyComponent->GetResource());
	
	ACrowdedGameMode* GameMode = GetWorld()->GetAuthGameMode<ACrowdedGameMode>();
	if (!GameMode)
		return;

	GameMode->UnregisterNPC(this);

	// Object
	if (CurrentObject)
		CurrentObject->Release(this);
	
	Destroy();
}
#pragma endregion

#pragma region Resources
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
#pragma endregion

#pragma region Action & Object
void ANPC::SetCurrentAction(const ENPCActionType NewAction)
{
	CurrentAction = NewAction;
	FString ActionString = StaticEnum<ENPCActionType>()->GetDisplayNameTextByValue(static_cast<int64>(CurrentAction)).ToString();
	
	OnCurrentActionChanged.Broadcast(CurrentAction);
}

void ANPC::SetCurrentObject(ABuildableObject* NewObject)
{
	CurrentObject = NewObject;
}
#pragma endregion

#pragma region Work / Train / Priority
void ANPC::SetWorkOnGeneratorType(const EProductionType NewType)
{
	if (WorkOnGeneratorType == NewType)
		return;
	
	WorkOnGeneratorType = NewType;

	CancelCurrentUse();
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

void ANPC::CancelCurrentUse() const
{
	if (ANPCController* ControllerNPC = Cast<ANPCController>(GetController()))
	{
		if (const UBlackboardComponent* Blackboard = ControllerNPC->GetBlackboardComponent())
		{
			if (const TObjectPtr<ABuildableObject> TargetObject = Cast<ABuildableObject>(Blackboard->GetValueAsObject("TargetObject")); TargetObject && TargetObject->GetObjectType() == EObjectType::Generator)
			{
				if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(ControllerNPC->GetBrainComponent()))
					BTComp->RestartTree();
			}
		}
	}
}

void ANPC::SetNPCPriorityType(const ENPCPriorityType NewType)
{
	if (NPCPriorityType == NewType)
		return;
	
	NPCPriorityType = NewType;

	CancelCurrentUse();
}

void ANPC::SetTrainingSkillType(const ETrainingSkillType NewType)
{
	if (TrainingSkillType == NewType)
		return;
	
	TrainingSkillType = NewType;

	CancelCurrentUse();
}
#pragma endregion

#pragma region Name & Color
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
#pragma endregion

#pragma region Selectable
void ANPC::OnSelected()
{
}

void ANPC::OnDeselected()
{
}
#pragma endregion

#pragma region Portrait
UTexture* ANPC::GetPortrait() const
{
	if (!PortraitRenderTarget)
		CapturePortrait();
	
	return PortraitRenderTarget;
}

void ANPC::CapturePortrait() const
{
	if (!PortraitCapture) return;

	const USkeletalMeshComponent* SourceMesh = GetMesh();
	if (!SourceMesh) return;
	
	USkeletalMeshComponent* Clone = NewObject<USkeletalMeshComponent>(const_cast<ANPC*>(this));

	if (!Clone) return;

	Clone->RegisterComponent();
	Clone->SetWorldTransform(SourceMesh->GetComponentTransform());
	
	Clone->SetSkeletalMesh(SourceMesh->GetSkeletalMeshAsset());
	const int32 MatCount = SourceMesh->GetNumMaterials();
	for (int32 i = 0; i < MatCount; i++)
	{
		Clone->SetMaterial(i, SourceMesh->GetMaterial(i));
	}
	
	Clone->SetRenderCustomDepth(false);
	Clone->SetOverlayMaterial(nullptr);
	
	PortraitCapture->ShowOnlyComponents.Empty();
	PortraitCapture->ShowOnlyComponent(Clone);
	PortraitCapture->CaptureScene();
	
	Clone->DestroyComponent();
}

bool ANPC::IsReadyForCapture() const
{
	return bReadyForCapture;
}

void ANPC::SetupCapture()
{
	FEngineShowFlags& Flags = PortraitCapture->ShowFlags;
	
	Flags.SetLighting(true);
	Flags.SetSkyLighting(true);
	Flags.SetDirectionalLights(true);
	Flags.SetPointLights(true);
	Flags.SetSpotLights(true);
	
	Flags.SetDynamicShadows(false);
	Flags.SetContactShadows(false);
	
	Flags.SetPostProcessing(false);
	Flags.SetTonemapper(false);
	Flags.SetBloom(false);
	Flags.SetEyeAdaptation(false);
	
	Flags.SetAtmosphere(false);
	Flags.SetFog(false);
	Flags.SetVolumetricFog(false);
	Flags.SetCloud(false);

	Flags.SetMaterials(true);
	Flags.SetSeparateTranslucency(true);
	Flags.SetTranslucency(true);
	
	PortraitRenderTarget = NewObject<UTextureRenderTarget2D>();

	PortraitRenderTarget->InitAutoFormat(512, 512);
	PortraitRenderTarget->ClearColor = FLinearColor(0,0,0,0);
	PortraitRenderTarget->RenderTargetFormat = RTF_RGBA8;
	PortraitRenderTarget->bAutoGenerateMips = false;
	
	PortraitCapture->TextureTarget = PortraitRenderTarget;
	
	bReadyForCapture = true;
	OnPlayerReadyForCapture.Broadcast();
}
#pragma endregion
