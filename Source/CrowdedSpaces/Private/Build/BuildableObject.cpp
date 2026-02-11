#include "Build/BuildableObject.h"

ABuildableObject::ABuildableObject()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComp;
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionObjectType(ECC_GameTraceChannel1); // Build
	MeshComp->SetCollisionResponseToAllChannels(ECR_Block);
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
	if (ComingNPC.IsValid())
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
	OnNPCUsingChanged.Broadcast(bHasNPCComing);
}

void ABuildableObject::StopUsing(ANPC* NPC)
{
	if (UsingNPC != NPC)
		return;
	
	UsingNPC = nullptr;
	
	bHasNPCUsing = false;
	OnNPCUsingChanged.Broadcast(bHasNPCComing);

	StopUsingImplementation();
}

void ABuildableObject::StartUsingImplementation()
{
}

void ABuildableObject::StopUsingImplementation()
{
}


