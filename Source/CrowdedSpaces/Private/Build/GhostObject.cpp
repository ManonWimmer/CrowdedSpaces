#include "Build/GhostObject.h"

AGhostObject::AGhostObject()
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComp);
	
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly); 
	MeshComp->SetCollisionObjectType(ECC_WorldDynamic);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore); 
	MeshComp->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	MeshComp->SetRenderCustomDepth(true);
}

void AGhostObject::SetMesh(UStaticMesh* InMesh) const
{
	if (InMesh && MeshComp)
	{
		MeshComp->SetStaticMesh(InMesh);
	}
}

FVector AGhostObject::GetMeshOffset() const
{
	return MeshComp ? MeshComp->GetRelativeLocation() : FVector::ZeroVector;
}

void AGhostObject::SetMaterials(TArray<UMaterialInterface*>& Materials) const
{
	if (!MeshComp) return;

	for (int32 i = 0; i < Materials.Num(); ++i)
	{
		MeshComp->SetMaterial(i, Materials[i]);
	}
}
