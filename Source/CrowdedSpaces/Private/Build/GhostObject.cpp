#include "Build/GhostObject.h"

AGhostObject::AGhostObject()
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly); 
	Mesh->SetCollisionObjectType(ECC_WorldDynamic);
	Mesh->SetCollisionResponseToAllChannels(ECR_Ignore); 
	Mesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	Mesh->SetRenderCustomDepth(true);
}

void AGhostObject::SetMesh(UStaticMesh* InMesh)
{
	if (InMesh && Mesh)
	{
		Mesh->SetStaticMesh(InMesh);
	}
}
