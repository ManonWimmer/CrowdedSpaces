#include "Build/BuildableObject.h"

#include "Build/BuildableRegistrySubsystem.h"
#include "Build/BuildSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerController.h"
#include "Player/CrowdedPlayerState.h"
#include "UI/GameHUD.h"

ABuildableObject::ABuildableObject()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComp;
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
	
	CrowdedPlayerState = CrowdedPlayerController->GetPlayerState<ACrowdedPlayerState>();
	if (!CrowdedPlayerState)
		return;

	GameHUD = Cast<AGameHUD>(CrowdedPlayerController->GetHUD());
	if (!GameHUD)
		return;

	BRS = GetWorld()->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BRS)
		return;
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

bool ABuildableObject::TryReserve(ANPC* NPC)
{
	if (ComingNPC.IsValid() && ComingNPC != NPC)
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

void ABuildableObject::DestroyObject()
{
	if (!BuildSubsystem)
		return;

	BuildSubsystem->RemoveObject(this);

	for (UBTTask_UseBuildableObject* Task : CurrentUsers)
	{
		if (Task)
		{
			Task->OnTargetDestroyed();
		}
	}

	GameHUD->HideCurrentSelectionWidget();
	
	Destroy();
}


