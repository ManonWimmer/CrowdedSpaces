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
	for (const USlotComponent* Slot : Slots)
	{
		if (Slot && Slot->IsFree())
		{
			return true;
		}
	}
	return false;
}

bool ABuildableObject::CanBeUsed() const
{
	return bCanBeUsed && bHasEnoughElectricity && bIsActivated;
}

void ABuildableObject::CheckCantBeUsedStopTask() const
{
	if (!CanBeUsed())
	{
		for (TWeakObjectPtr<ANPC> NPC : UsingNPCs)
		{
			if (!NPC.IsValid())
				continue;

			if (const UBTTask_UseBuildableObject* Task = NPC->GetCurrentUseTask())
			{
				CS_LOG("Force stopping task for NPC: %s", *GetNameSafe(NPC.Get()));
				//FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}
		}
	}
}

int ABuildableObject::HasNPCComing() const
{
	return 0;
}

int ABuildableObject::HasNPCUsing() const
{
	return 0;
}

int ABuildableObject::GetSlotsNbr() const
{
	return Slots.Num();
}

int ABuildableObject::GetFreeSlotsNbr() const
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

bool ABuildableObject::TryReserve(const ANPC* NPC)
{
	CS_LOG("TryReserve by NPC: %s",
		*GetNameSafe(NPC));

	if (GetFreeSlot() != nullptr)
	{
		CS_LOG("RESERVE SUCCESS");
	}
	else
	{
		CS_LOG("RESERVE FAILED");
	}
	
	return GetFreeSlot() != nullptr;
}

void ABuildableObject::Release(ANPC* NPC)
{
	ReleaseSlot(NPC);
	OnSlotsUpdated.Broadcast();

	CS_LOG("Released by NPC: %s", *GetNameSafe(NPC));

	NPC->SetCurrentObject(nullptr);
}

void ABuildableObject::StartUsing(ANPC* NPC)
{
	CS_LOG("START USING SUCCESS NPC: %s", *GetNameSafe(NPC));

	StartUsingImplementation(NPC);
	
	//OnNPCComingChanged.Broadcast(bHasNPCComing);
	//OnNPCUsingChanged.Broadcast(bHasNPCUsing);
}

void ABuildableObject::StopUsing(ANPC* NPC)
{
	CS_LOG("StopUsing NPC: %s | Current UsingNPC: %s",
		*GetNameSafe(NPC));
	
	StopUsingImplementation(NPC);
	
	CS_LOG("STOP USING SUCCESS");
	
	//OnNPCUsingChanged.Broadcast(bHasNPCUsing);
}

bool ABuildableObject::StartUsingImplementation(ANPC* NPC)
{
	return true; 
}

bool ABuildableObject::StopUsingImplementation(ANPC* NPC)
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
	
	for (TWeakObjectPtr<ANPC> NPC : UsingNPCs)
	{
		if (!NPC.IsValid())
			continue;

		if (UBTTask_UseBuildableObject* Task = NPC->GetCurrentUseTask())
		{
			CS_LOG("DestroyObject: stopping task for NPC: %s", *GetNameSafe(NPC.Get()));
			//Task->OnTargetDestroyed();
		}
		
		NPC->SetCurrentObject(nullptr);
	}
	
	UsingNPCs.Empty();

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
			Slot->Acquire(NPC);
			OnSlotsUpdated.Broadcast();
			
			CS_LOG("Slot reserved: %s by NPC: %s",
				*Slot->GetName(),
				*GetNameSafe(NPC));
			
			CS_LOG("RESERVED SUCCESS by NPC: %s", *GetNameSafe(NPC));
			
			NPC->SetCurrentObject(this);
			UsingNPCs.Add(NPC);
			
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
			Slot->Release(NPC);
			OnSlotsUpdated.Broadcast();
			UsingNPCs.Remove(NPC);
			return;
		}
	}
	
	CS_LOG_WARNING("WARNING: No slot found for NPC");
}


