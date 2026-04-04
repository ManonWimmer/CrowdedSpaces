#include "Build/BuildableObject.h"

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

#pragma region Mesh
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
#pragma endregion

#pragma region Slot Reservation
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

USlotComponent* ABuildableObject::ReserveSpecificSlot(ANPC* NPC, USlotComponent* Slot)
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

void ABuildableObject::Release(ANPC* NPC)
{
	ReleaseSlot(NPC);
	OnSlotsUpdated.Broadcast();

	CS_LOG("Released by NPC: %s", *GetNameSafe(NPC));

	NPC->SetCurrentObject(nullptr);
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
#pragma endregion 

#pragma region Used & Activated
bool ABuildableObject::CanBeUsed() const
{
	return bCanBeUsed && bHasEnoughElectricity && bIsActivated;
}

void ABuildableObject::SetHasEnoughElectricity(const bool bEnoughElectricity)
{
	bHasEnoughElectricity = bEnoughElectricity;
}

void ABuildableObject::SetIsActivated(const bool bActivated)
{
	bIsActivated = bActivated;
}
#pragma endregion

#pragma region Use Object
void ABuildableObject::StartUsing(ANPC* NPC)
{
	CS_LOG("START USING SUCCESS NPC: %s", *GetNameSafe(NPC));

	StartUsingImplementation(NPC);
	NPC->SetCurrentAction(NPCAction);
}

void ABuildableObject::StopUsing(ANPC* NPC)
{
	CS_LOG("StopUsing NPC: %s | Current UsingNPC: %s",
		*GetNameSafe(NPC));

	ReleaseSlot(NPC);
	StopUsingImplementation(NPC);
	NPC->SetCurrentAction(ENPCActionWidget::Idle);
	
	CS_LOG("STOP USING SUCCESS");
}

bool ABuildableObject::StartUsingImplementation(ANPC* NPC)
{
	return true; 
}

bool ABuildableObject::StopUsingImplementation(ANPC* NPC)
{
	return true;
}
#pragma endregion

#pragma region Destroy
void ABuildableObject::DestroyObject()
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
#pragma endregion