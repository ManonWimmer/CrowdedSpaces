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


