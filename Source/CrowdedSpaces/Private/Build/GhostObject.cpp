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

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialFinder(TEXT("/Game/Project/Assets/Materials/M_Ghost.M_Ghost"));
	if(MaterialFinder.Succeeded())
	{
		GhostBaseMaterial = MaterialFinder.Object;
	}
	Mesh->SetRenderCustomDepth(true);
}

void AGhostObject::SetMesh(UStaticMesh* InMesh)
{
	if (InMesh && Mesh)
	{
		Mesh->SetStaticMesh(InMesh);
		MeshExtent = InMesh->GetBounds().BoxExtent;
	}
}

void AGhostObject::SetValid(bool bIsValid)
{
	if(!Mesh || !GhostBaseMaterial) return;

	if(DynamicMat == nullptr)
	{
		DynamicMat = UMaterialInstanceDynamic::Create(GhostBaseMaterial, this);
		Mesh->SetMaterial(0, DynamicMat);
	}

	FLinearColor Color = bIsValid ? FLinearColor::Green : FLinearColor::Red;
	DynamicMat->SetVectorParameterValue("Color", Color);
}
