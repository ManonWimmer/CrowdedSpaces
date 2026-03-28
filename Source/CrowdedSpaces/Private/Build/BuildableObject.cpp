#include "Build/BuildableObject.h"

#include "Build/BuildableRegistrySubsystem.h"
#include "Build/BuildSubsystem.h"
#include "Build/SlotComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerController.h"
#include "Game/CrowdedGameState.h"
#include "Player/PlayerFunctionLibrary.h"
#include "UI/GameHUD.h"

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

	for (USlotComponent* Slot : Slots)
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
	if (ComingNPC.IsValid() && ComingNPC != NPC && !CanBeUsed())
		return false;

	ComingNPC = NPC;
	
	bHasNPCComing = true;
	OnNPCComingChanged.Broadcast(bHasNPCComing);
	
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
	ReleaseSlot(NPC);
	
	bHasNPCComing = false;
	OnNPCComingChanged.Broadcast(bHasNPCComing);
}

void ABuildableObject::StartUsing(ANPC* NPC)
{
	if (ComingNPC != NPC)
		return;
	
	ComingNPC = nullptr;
	UsingNPC = NPC;
	
	bHasNPCComing = false;
	OnNPCComingChanged.Broadcast(bHasNPCComing);

	bHasNPCUsing = true;
	OnNPCUsingChanged.Broadcast(bHasNPCUsing);
}

void ABuildableObject::StopUsing(ANPC* NPC)
{
	if (UsingNPC != NPC)
		return;
	
	UsingNPC = nullptr;
	ReleaseSlot(NPC);
	
	bHasNPCUsing = false;
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
	
	UResourceComponent* PlayerMoneyComponent = UPlayerFunctionLibrary::GetPlayerResourceComponent(this, EResourceType::Money);
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
	for (USlotComponent* Slot : Slots)
	{
		if (Slot && Slot->IsFree())
		{
			Slot->SetOccupied(true, NPC);
			return Slot;
		}
	}
	return nullptr;
}

void  ABuildableObject::ReleaseSlot(ANPC* NPC)
{
	for (USlotComponent* Slot : Slots)
	{
		if (Slot && Slot->OccupyingNPC == NPC)
		{
			Slot->SetOccupied(false, nullptr);
			return;
		}
	}
}


