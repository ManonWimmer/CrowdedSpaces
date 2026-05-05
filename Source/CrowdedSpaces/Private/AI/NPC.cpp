#include "AI/NPC.h"

#include "Action/Action.h"
#include "Action/ActionComponent.h"
#include "AI/NameGeneratorSubsystem.h"
#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Object/UsableObject.h"
#include "Build/SlotComponent.h"
#include "Object/Buildable/BuildableGenerator.h"
#include "Object/Buildable/BuildableTrainingStation.h"
#include "Camera/FreeCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SpotLightComponent.h"
#include "Debug/CrowdedSpacesLogs.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Game/CrowdedGameMode.h"
#include "Game/CrowdedGameState.h"
#include "Kismet/KismetMathLibrary.h"
#include "Training/TrainingData.h"
#include "Training/TrainingSubsystem.h"
#include "UI/UIUtils.h"
#include "UI/Widgets/World/FoodBarWidget.h"
#include "UI/Widgets/World/NPCActionWidget.h"
#include "UI/Widgets/Selection/NPCNameWidget.h"

#define BO_LOG(Format, ...) UE_LOG(LogTemp, Warning, TEXT("[BuildableObject:%s] " Format), *GetNameSafe(this), ##__VA_ARGS__)

ANPC::ANPC()
{
	// Food
	FoodComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("FoodComponent"));
	FoodComponent->SetType(EResourceType::Food);
	FoodComponent->SetCanLoseAndRegenResource(true);
	
	NPCNameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NPCNameWidget"));
	NPCNameWidget->SetupAttachment(GetMesh());

	NPCActionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("NPCActionWidget"));
	NPCActionWidget->SetupAttachment(GetMesh());

	// Energy
	EnergyComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("EnergyComponent"));
	EnergyComponent->SetType(EResourceType::Energy);
	EnergyComponent->SetCanLoseAndRegenResource(true);

	// Health
	HealthComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("HealthComponent"));
	HealthComponent->SetType(EResourceType::Health);
	HealthComponent->SetCanLoseAndRegenResource(false);

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
	PortraitCapture->bAlwaysPersistRenderingState = true;

	// Portrait light
	PortraitLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("PortraitLight"));
	PortraitLight->SetupAttachment(GetMesh());

	PortraitLight->SetVisibility(false); 
	
	PortraitLight->Intensity = 5000.f;
	PortraitLight->SetCastShadows(false);
	PortraitLight->SetMobility(EComponentMobility::Movable);
}

void ANPC::OnNPCSelected_Implementation()
{
}

void ANPC::OnNPCDeselected_Implementation()
{
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();

	// Camera
	PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;

	FreeCameraPawn = Cast<AFreeCameraPawn>(PlayerController->GetPawn());
	if (!FreeCameraPawn) return;

	CameraComponent = FreeCameraPawn->GetCameraComponent();
	if (!CameraComponent) return;
	
	// Die
	FoodComponent->OnNoMoreResource.AddDynamic(this, &ANPC::OnDead);
	EnergyComponent->OnNoMoreResource.AddDynamic(this, &ANPC::OnDead);
	HealthComponent->OnNoMoreResource.AddDynamic(this, &ANPC::OnDead);
	HealthComponent->OnResourceAdded.AddDynamic(this, &ANPC::OnHealed);
	HealthComponent->OnResourceRemoved.AddDynamic(this, &ANPC::OnDamaged);

	ACrowdedGameMode* GameMode = GetWorld()->GetAuthGameMode<ACrowdedGameMode>();
	if (!GameMode)
		return;

	GameMode->RegisterNPC(this);

	GameState = GetWorld()->GetGameState<ACrowdedGameState>();

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

	RandomColor = GetRandomColor();
	
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
	ANPCController* ControllerNPC = Cast<ANPCController>(GetController());
	if (!ControllerNPC)
	return;

	Blackboard = ControllerNPC->GetBlackboardComponent();
	if (!Blackboard)
		return;
			
	InitActions();

	// Portrait
	SetupCapture();
}

void ANPC::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FoodComponent->OnNoMoreResource.RemoveDynamic(this, &ANPC::OnDead);
	EnergyComponent->OnNoMoreResource.RemoveDynamic(this, &ANPC::OnDead);
	HealthComponent->OnNoMoreResource.RemoveDynamic(this, &ANPC::OnDead);
	HealthComponent->OnResourceAdded.RemoveDynamic(this, &ANPC::OnHealed);
	HealthComponent->OnResourceRemoved.RemoveDynamic(this, &ANPC::OnDamaged);
	
	Super::EndPlay(EndPlayReason);
}

void ANPC::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FUIUtils::RotateComponentToCameraYaw(PlayerController, NPCNameWidget);
	FUIUtils::RotateComponentToCameraYaw(PlayerController, NPCActionWidget);
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
				
				TrainingSkillsExp[ETrainingSkillType::ElectricityProduction] += AddExp;

				if (TrainingSkillsExp[ETrainingSkillType::ElectricityProduction] > CurrentLevelExpNeeded)
				{
					TrainingSkillsExp[ETrainingSkillType::ElectricityProduction] = 0;
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
	// Map qui renvoyait pas bon pour le health component...
	
	switch (Type)
	{
	case EResourceType::Food:
		return FoodComponent;
	case EResourceType::Energy:
		return EnergyComponent;
	case EResourceType::Health:
		return HealthComponent;
	default:
		return nullptr;
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

void ANPC::SetCurrentObject(AUsableObject* NewObject)
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

void ANPC::CancelCurrentUse()
{
	if (!Blackboard)
		return;

	ANPCController* ControllerNPC = Cast<ANPCController>(GetController());
	if (!ControllerNPC)
		return;
	
	if (const TObjectPtr<AUsableObject> TargetObject = Cast<AUsableObject>(Blackboard->GetValueAsObject("TargetObject")); TargetObject && TargetObject->GetObjectType() == EObjectType::Generator)
	{
		if (UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(ControllerNPC->GetBrainComponent()))
			BTComp->RestartTree();

		StopAction();
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

			OnNameSet.Broadcast();
			
			return;
		}
	}

	// Retry dans 0.1s
	GetWorld()->GetTimerManager().SetTimer(
		NameRetryTimer,
		this,
		&ANPC::TryGenerateName,
		0.1f,
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

#pragma region Actions
void ANPC::InitActions()
{
	if (!GameState)
		return;
	
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

	PortraitLight->SetVisibility(true);

	Clone->RegisterComponent();
	Clone->SetWorldTransform(SourceMesh->GetComponentTransform());
	
	Clone->SetSkeletalMesh(SourceMesh->GetSkeletalMeshAsset());
	const int32 MatCount = SourceMesh->GetNumMaterials();
	for (int32 i = 0; i < MatCount; i++)
	{
		Clone->SetMaterial(i, SourceMesh->GetMaterial(i));
	}
	
	Clone->SetRenderCustomDepth(false);
	//Clone->SetOverlayMaterial(nullptr);
	Clone->CastShadow = false;
	
	PortraitCapture->ShowOnlyComponents.Empty();
	PortraitCapture->ShowOnlyComponent(Clone);
	PortraitCapture->CaptureScene();
	
	Clone->DestroyComponent();

	PortraitLight->SetVisibility(false);
}

bool ANPC::IsReadyForCapture() const
{
	return bReadyForCapture;
}

void ANPC::SetupCapture()
{
	FEngineShowFlags& Flags = PortraitCapture->ShowFlags;
	
	Flags.SetPostProcessing(false);
	Flags.SetBloom(false);
	Flags.SetFog(false);
	Flags.SetAtmosphere(false);
	Flags.SetVolumetricFog(false);
	Flags.SetCloud(false);

	Flags.SetLighting(true);
	Flags.SetDirectionalLights(true);
	Flags.SetPointLights(true);
	Flags.SetSpotLights(true);

	Flags.SetMaterials(true);
	Flags.SetTranslucency(true);
	Flags.SetSeparateTranslucency(true);
	
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

#pragma region Action Object
void ANPC::SetActionObject(AUsableObject* Object)
{
	if (!Blackboard)
		return;
	
	if (!Object)
		return;
	
	USlotComponent* Slot = Object->GetNearestFreeAndWalkableSlot(this, GetActorLocation());
	if (!Slot)
		return;
	
	Blackboard->SetValueAsObject("ActionObject", Object);
	Blackboard->SetValueAsObject("ActionObjectSlot", Slot);
	Blackboard->SetValueAsVector("ActionObjectLocation", Slot->GetComponentLocation());

	FocusCameraOnActionObject();
	
	OnActionObjectChanged.Broadcast();
}

void ANPC::StopAction() const
{
	CS_LOG_WARNING("Stop action behavior tree reset");
	
	Blackboard->SetValueAsObject("ActionObject", nullptr);
	Blackboard->SetValueAsObject("ActionObjectSlot", nullptr);
	Blackboard->SetValueAsVector("ActionObjectLocation", FVector::Zero());

	FocusCameraOnNPC();
	
	OnActionObjectChanged.Broadcast();
}

AUsableObject* ANPC::GetActionObject() const
{
	if (!Blackboard)
		return nullptr;
	
	return Cast<AUsableObject>(Blackboard->GetValueAsObject("ActionObject"));
}

bool ANPC::HasActionObject() const
{
	return GetActionObject() != nullptr;
}
#pragma endregion

#pragma region Camera
void ANPC::FocusCameraOnActionObject() const
{
	const AUsableObject* ActionObject = GetActionObject();
	if (!ActionObject)
		return;

	FreeCameraPawn->FocusOnActor(ActionObject);
}

void ANPC::FocusCameraOnNPC() const
{
	FreeCameraPawn->FocusOnActor(this);
}
#pragma endregion

#pragma region Auto Needs
bool ANPC::HasAutoNeeds() const
{
	return bAutoNeeds;
}

void ANPC::SetAutoNeeds(const bool bNewAutoNeeds)
{
	bAutoNeeds = bNewAutoNeeds;
}
#pragma endregion

#pragma region Health
void ANPC::OnDamaged()
{
	OnDamagedFeedback();

	// Flee
	StopAction();
	Blackboard->SetValueAsBool("bIsFleeing", true);
}

void ANPC::OnHealed()
{
	OnHealedFeedback();
}

void ANPC::OnDead()
{
	OnDeadFeedback();
	Die(); // mettre die dans feedback bp plus tard pour play sound, vfx etc sans null ref si il est direct destroy
}

void ANPC::OnHealedFeedback_Implementation()
{
}

void ANPC::OnDamagedFeedback_Implementation()
{
}

void ANPC::OnDeadFeedback_Implementation()
{
}
#pragma endregion
