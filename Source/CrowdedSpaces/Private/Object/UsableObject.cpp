#include "Object/UsableObject.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Action/Action.h"
#include "Action/ActionComponent.h"
#include "AI/NPCController.h"
#include "Build/BuildableRegistrySubsystem.h"
#include "Build/BuildSubsystem.h"
#include "Build/SlotComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerController.h"
#include "Game/CrowdedGameState.h"
#include "Player/PlayerHelpers.h"
#include "UI/GameHUD.h"
#include "Debug/CrowdedSpacesLogs.h"
#include "Game/CrowdedGameInstance.h"

AUsableObject::AUsableObject()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionObjectType(ECC_GameTraceChannel1); // Build
	MeshComp->SetCollisionResponseToAllChannels(ECR_Block);

	// Actions
	ActionComponent = CreateDefaultSubobject<UActionComponent>("ActionComponent");

	// Health
	HealthComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("HealthComponent"));
	HealthComponent->SetType(EResourceType::Health);
	HealthComponent->SetCanLoseAndRegenResource(false);
}

void AUsableObject::BeginPlay()
{
	Super::BeginPlay();

	BuildSubsystem = GetWorld()->GetSubsystem<UBuildSubsystem>();
	if (!BuildSubsystem)
		return;
	
	CrowdedPlayerController = Cast<ACrowdedPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!CrowdedPlayerController)
		return;

	const UWorld* World = GetWorld();
	if (!World)
		return;
	
	GameState = World->GetGameState<ACrowdedGameState>();
	if (!GameState)
		return;

	GameHUD = Cast<AGameHUD>(CrowdedPlayerController->GetHUD());
	if (!GameHUD)
		return;

	BRS = World->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BRS)
		return;

	// Setup interaction slots
	Slots.Empty();

	GetComponents<USlotComponent>(Slots);

	for (const USlotComponent* Slot : Slots)
	{
		if (!Slot) continue;

		UE_LOG(LogTemp, Warning, TEXT("Slot found: %s"), *Slot->GetName());
	}

	// Materials
	const UCrowdedGameInstance* GameInstance = Cast<UCrowdedGameInstance>(World->GetGameInstance());
	if (!GameInstance)
		return;
	
	DisabledMaterial = GameInstance->DisabledObjectsMaterial;
	WillBeRemovedMaterial = GameInstance->WillBeRemovedObjectsMaterial;
	NormalMaterial = MeshComp->GetMaterial(0);

	// Actions
	InitActions();
}

#pragma region Mesh
void AUsableObject::SetMesh(UStaticMesh* Mesh) const
{
	MeshComp->SetStaticMesh(Mesh);
}

FVector AUsableObject::GetExtent() const
{
	if(MeshComp && MeshComp->GetStaticMesh())
    {
        return MeshComp->Bounds.BoxExtent;
    }
	
    return FVector::ZeroVector;
}
#pragma endregion

#pragma region Slot Reservation
USlotComponent* AUsableObject::GetNearestFreeAndWalkableSlot(ANPC* NPC, const FVector& FromLocation)
{
	USlotComponent* BestSlot = nullptr;
	float BestDist = FLT_MAX;

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSys || !NPC)
		return nullptr;

	for (USlotComponent* Slot : Slots)
	{
		if (!Slot || !Slot->IsFree())
			continue;

		const FVector SlotLocation = Slot->GetComponentLocation();

		// Check path validity
		const UNavigationPath* Path = NavSys->FindPathToLocationSynchronously(GetWorld(),FromLocation, SlotLocation, NPC);

		if (!Path || !Path->IsValid())
			continue;
		
		const float PathLength = Path->GetPathLength();

		if (PathLength < BestDist)
		{
			BestDist = PathLength;
			BestSlot = Slot;
		}
	}

	return BestSlot;
}

bool AUsableObject::IsAvailableForReservation(const ANPC* NPC) const
{
	for (const USlotComponent* Slot : Slots)
	{
		if (Slot && Slot->IsFree())
		{
			return true;
		}
	}
	return false;
}

int AUsableObject::GetSlotsNbr() const
{
	return Slots.Num();
}

int AUsableObject::GetFreeSlotsNbr() const
{
	int FreeSlots = 0;
	
	for (const USlotComponent* Slot : Slots)
	{
		if (Slot && Slot->IsFree())
		{
			FreeSlots++;
		}
	}
	
	return FreeSlots;
}

USlotComponent* AUsableObject::ReserveSpecificSlot(ANPC* NPC, USlotComponent* Slot)
{
	if (!Slot || !Slot->IsFree())
		return nullptr;
	
	if (UsingNPCs.Contains(NPC))
		return nullptr;

	Slot->Acquire(NPC);

	UsingNPCs.Add(NPC);
	NPC->SetCurrentObject(this);

	OnSlotsUpdated.Broadcast();

	return Slot;
}

void AUsableObject::Release(ANPC* NPC)
{
	ReleaseSlot(NPC);
	OnSlotsUpdated.Broadcast();

	CS_LOG("Released by NPC: %s", *GetNameSafe(NPC));

	NPC->SetCurrentObject(nullptr);
}

void  AUsableObject::ReleaseSlot(ANPC* NPC)
{
	CS_LOG("ReleaseSlot NPC: %s", *GetNameSafe(NPC));
	
	for (USlotComponent* Slot : Slots)
	{
		if (Slot && Slot->OccupyingNPC == NPC)
		{
			CS_LOG("Slot released: %s", *Slot->GetName());
			Slot->Release(NPC);
			OnSlotsUpdated.Broadcast();
			UsingNPCs.Remove(NPC);
			return;
		}
	}
	
	CS_LOG_WARNING("WARNING: No slot found for NPC");
}
#pragma endregion 

#pragma region Used & Activated
bool AUsableObject::CanBeUsed() const
{
	return bCanBeUsed && bHasEnoughElectricity && bIsActivated;
}

void AUsableObject::SetHasEnoughElectricity(const bool bEnoughElectricity)
{
	bHasEnoughElectricity = bEnoughElectricity;

	UpdateMaterialState();
}

void AUsableObject::SetIsActivated(const bool bActivated)
{
	bIsActivated = bActivated;

	UpdateMaterialState();
}

void AUsableObject::SetWillBeRemoved(const bool bRemoved)
{
	if (bWillBeRemoved == bRemoved)
		return;
	
	bWillBeRemoved = bRemoved;

	UpdateMaterialState();
}

void AUsableObject::UpdateMaterialState() const
{
	if (bWillBeRemoved)
	{
		MeshComp->SetMaterial(0, WillBeRemovedMaterial);
	}
	else if (!CanBeUsed())
	{
		MeshComp->SetMaterial(0, DisabledMaterial);
	}
	else
	{
		MeshComp->SetMaterial(0, NormalMaterial);
	}
}
#pragma endregion

#pragma region Use Object
void AUsableObject::StartUsing(ANPC* NPC)
{
	CS_LOG("START USING SUCCESS NPC: %s", *GetNameSafe(NPC));

	StartUsingImplementation(NPC);
	NPC->SetCurrentAction(NPCUseAction);
}

void AUsableObject::StopUsing(ANPC* NPC)
{
	CS_LOG("StopUsing NPC: %s | Current UsingNPC: %s",
		*GetNameSafe(NPC));

	ReleaseSlot(NPC);
	StopUsingImplementation(NPC);
	NPC->SetCurrentAction(ENPCActionType::Idle);
	
	CS_LOG("STOP USING SUCCESS");
}

bool AUsableObject::StartUsingImplementation(ANPC* NPC)
{
	return true; 
}

bool AUsableObject::StopUsingImplementation(ANPC* NPC)
{
	return true;
}
#pragma endregion

#pragma region Destroy
void AUsableObject::DestroyObject()
{
	if (!BuildSubsystem)
		return;

	bIsBeingDestroyed = true;
	
	BuildSubsystem->RemoveObject(this);
	
	UResourceComponent* PlayerMoneyComponent = PlayerHelpers::GetPlayerResourceComponent(*GetWorld(), EResourceType::Money);
	PlayerMoneyComponent->AddResource(BuildData->DestroyMoney);
	
	TArray<TWeakObjectPtr<ANPC>> NPCsCopy = UsingNPCs;

	for (TWeakObjectPtr<ANPC> NPC : NPCsCopy)
	{
		if (!NPC.IsValid())
			continue;

		StopUsing(NPC.Get());

		if (ANPCController* Controller = Cast<ANPCController>(NPC->GetController()))
		{
			if (UBehaviorTreeComponent* BT = Cast<UBehaviorTreeComponent>(Controller->GetBrainComponent()))
			{
				BT->StopTree(EBTStopMode::Safe);
				BT->RestartTree();
			}
		}
	}
	
	UsingNPCs.Empty();

	GameHUD->HideCurrentSelectionWidget();
	
	Destroy();
}

bool AUsableObject::IsOverlappingCells(const TSet<FIntPoint>& Cells) const
{
	for (const FIntPoint& Cell : OccupiedCells)
	{
		if (Cells.Contains(Cell))
		{
			return true;
		}
	}
	return false;
}
#pragma endregion

#pragma region Actions
void AUsableObject::InitActions()
{
}
#pragma endregion