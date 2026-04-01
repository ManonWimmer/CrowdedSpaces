#include "Build/BuildableObject.h"

#include "Build/BuildableRegistrySubsystem.h"
#include "Build/BuildSubsystem.h"
#include "Build/SlotComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerController.h"
#include "Game/CrowdedGameState.h"
#include "Player/PlayerHelpers.h"
#include "UI/GameHUD.h"
#include "Debug/CrowdedSpacesLogs.h"

ABuildableObject::ABuildableObject()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionObjectType(ECC_GameTraceChannel1); // Build
	MeshComp->SetCollisionResponseToAllChannels(ECR_Block);
}

void ABuildableObject::BeginPlay()
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
	
}

void ABuildableObject::SetMesh(UStaticMesh* Mesh) const
{
	MeshComp->SetStaticMesh(Mesh);
}

FVector ABuildableObject::GetExtent() const
{
	if(MeshComp && MeshComp->GetStaticMesh())
    {
        return MeshComp->Bounds.BoxExtent;
    }
	
    return FVector::ZeroVector;
}

USlotComponent* ABuildableObject::GetNearestFreeSlot(const FVector& FromLocation)
{
	USlotComponent* BestSlot = nullptr;
	float BestDist = FLT_MAX;

	for (USlotComponent* Slot : Slots)
	{
		if (!Slot || !Slot->IsFree())
			continue;

		float Dist = FVector::Dist(FromLocation, Slot->GetComponentLocation());

		if (Dist < BestDist)
		{
			BestDist = Dist;
			BestSlot = Slot;
		}
	}

	return BestSlot;
}

bool ABuildableObject::IsAvailableForReservation(const ANPC* NPC) const
{
	return (!ComingNPC.IsValid() || ComingNPC == NPC)&& !bHasNPCUsing;
}

bool ABuildableObject::CanBeUsed() const
{
	return bCanBeUsed && bHasEnoughElectricity && bIsActivated;
}

void ABuildableObject::CheckCantBeUsedStopTask() const
{
	if (!CanBeUsed())
	{
		if (bHasNPCUsing && UsingNPC.IsValid())
		{
			if (CurrentTask)
			{
				CS_LOG("Force stopping task for NPC: %s", *GetNameSafe(UsingNPC.Get()));
				CurrentTask->ForceStopTask();
			}
		}
	}
}

void ABuildableObject::SetHasEnoughElectricity(const bool bEnoughElectricity)
{
	bHasEnoughElectricity = bEnoughElectricity;

	CheckCantBeUsedStopTask();
}

void ABuildableObject::SetIsActivated(const bool bActivated)
{
	bIsActivated = bActivated;

	CheckCantBeUsedStopTask();
}

bool ABuildableObject::TryReserve(ANPC* NPC)
{
	CS_LOG("TryReserve by NPC: %s | Current ComingNPC: %s",
		*GetNameSafe(NPC),
		*GetNameSafe(ComingNPC.Get()));

	if (bHasNPCUsing)
	{
		CS_LOG("RESERVE FAILED: has npc using");
		return false;
	}

	if (ComingNPC.IsValid() && ComingNPC != NPC)
	{
		CS_LOG("RESERVE FAILED: already reserved by other NPC");
		return false;
	}

	ComingNPC = NPC;
	bHasNPCComing = true;
	CS_LOG("RESERVED SUCCESS by NPC: %s", *GetNameSafe(NPC));
	
	return true;
}

bool ABuildableObject::IsReservedByOther(TObjectPtr<ANPC> NPC) const
{
	return ComingNPC.IsValid() && ComingNPC != NPC;
}

void ABuildableObject::Release(ANPC* NPC)
{
	if (ComingNPC != NPC)
		return;
		
	ComingNPC = nullptr;
	bHasNPCComing = false;
	
	UsingNPC = nullptr;
	bHasNPCUsing = false;
	
	ReleaseSlot(NPC);

	OnNPCUsingChanged.Broadcast(bHasNPCUsing);
	OnNPCComingChanged.Broadcast(bHasNPCComing);

	BO_LOG("Released by NPC: %s", *GetNameSafe(NPC));

	NPC->SetCurrentObject(nullptr);
}

void ABuildableObject::StartUsing(ANPC* NPC)
{
	CS_LOG("StartUsing attempt NPC: %s | ComingNPC: %s | UsingNPC: %s",
		*GetNameSafe(NPC),
		*GetNameSafe(ComingNPC.Get()),
		*GetNameSafe(UsingNPC.Get()));
	
	if (ComingNPC != NPC)
	{
		CS_LOG("START USING FAILED: ComingNPC mismatch");
		return;
	}
	
	ComingNPC = nullptr;
	UsingNPC = NPC;
	
	bHasNPCComing = false;
	bHasNPCUsing = true;

	CS_LOG("START USING SUCCESS NPC: %s", *GetNameSafe(NPC));
	
	OnNPCComingChanged.Broadcast(bHasNPCComing);
	OnNPCUsingChanged.Broadcast(bHasNPCUsing);
}

void ABuildableObject::StopUsing(ANPC* NPC)
{
	CS_LOG("StopUsing NPC: %s | Current UsingNPC: %s",
		*GetNameSafe(NPC),
		*GetNameSafe(UsingNPC.Get()));
	
	if (UsingNPC != NPC)
	{
		CS_LOG("STOP USING IGNORED (wrong NPC)");
		return;
	}
	
	UsingNPC = nullptr;
	ReleaseSlot(NPC);

	bHasNPCUsing = false;
	ComingNPC = nullptr;
	bHasNPCComing = false;
	
	CS_LOG("STOP USING SUCCESS");
	
	OnNPCUsingChanged.Broadcast(bHasNPCUsing);
}

bool ABuildableObject::StartUsingImplementation(UBTTask_UseBuildableObject* UseObjectTask)
{
	return true; 
}

bool ABuildableObject::StopUsingImplementation(UBTTask_UseBuildableObject* UseObjectTask)
{
	return true;
}

void ABuildableObject::SetBuildData(UBuildData* NewData)
{
	BuildData = NewData;
}

void ABuildableObject::DestroyObject()
{
	if (!BuildSubsystem)
		return;
	
	BuildSubsystem->RemoveObject(this);
	
	UResourceComponent* PlayerMoneyComponent = PlayerHelpers::GetPlayerResourceComponent(*GetWorld(), EResourceType::Money);
	PlayerMoneyComponent->AddResource(BuildData->DestroyMoney);

	if (CurrentTask)
	{
		CurrentTask->OnTargetDestroyed();
	}

	GameHUD->HideCurrentSelectionWidget();
	
	Destroy();
}

USlotComponent* ABuildableObject::GetFreeSlot()
{
	for (USlotComponent* Slot : Slots)
	{
		if (Slot && Slot->IsFree())
		{
			return Slot;
		}
	}
	return nullptr;
}

USlotComponent* ABuildableObject::ReserveSlot(ANPC* NPC)
{
	CS_LOG("ReserveSlot  NPC: %s", *GetNameSafe(NPC));
	
	for (USlotComponent* Slot : Slots)
	{
		if (Slot && Slot->IsFree())
		{
			Slot->SetOccupied(true, NPC);
			
			CS_LOG("Slot reserved: %s by NPC: %s",
				*Slot->GetName(),
				*GetNameSafe(NPC));
			
			ComingNPC = NPC;
			bHasNPCComing = true;
			BO_LOG("RESERVED SUCCESS by NPC: %s", *GetNameSafe(NPC));
	
			OnNPCComingChanged.Broadcast(bHasNPCComing);

			NPC->SetCurrentObject(this);
			
			return Slot;
		}
	}

	CS_LOG("ReserveSlot FAILED: no free slot");
	
	return nullptr;
}

void  ABuildableObject::ReleaseSlot(ANPC* NPC)
{
	CS_LOG("ReleaseSlot NPC: %s", *GetNameSafe(NPC));
	
	for (USlotComponent* Slot : Slots)
	{
		if (Slot && Slot->OccupyingNPC == NPC)
		{
			CS_LOG("Slot released: %s", *Slot->GetName());
			Slot->SetOccupied(false, nullptr);
			return;
		}
	}
	
	CS_LOG_WARNING("WARNING: No slot found for NPC");
}


