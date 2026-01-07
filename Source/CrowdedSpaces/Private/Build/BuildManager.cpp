#include "Build/BuildManager.h"
#include "Build/BuildableObject.h"

ABuildManager::ABuildManager(): CurrentGhost(nullptr), CurrentGhostMesh(nullptr), DefaultBuildData(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	SnapSize = 100.f;
}

void ABuildManager::BeginPlay()
{
	Super::BeginPlay();
	if(DefaultBuildData)
	{
		StartBuilding(DefaultBuildData->Mesh);
	}
}

void ABuildManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateGhost();
}

void ABuildManager::StartBuilding(UStaticMesh* Mesh)
{
	if(!Mesh) return;

	if(!CurrentGhost)
	{
		CurrentGhost = GetWorld()->SpawnActor<AGhostObject>(AGhostObject::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	}

	CurrentGhost->SetMesh(Mesh);
	CurrentGhostMesh = Mesh;
}

void ABuildManager::PlaceObject() const
{
	if(!CurrentGhost || !CurrentGhostMesh) return;

	FVector Location = CurrentGhost->GetActorLocation();
	FVector Extent = CurrentGhost->GetMeshExtent();
    
	if(!CanPlace(Location, Extent))
		return;
	
	ABuildableObject* Placed = GetWorld()->SpawnActor<ABuildableObject>(ABuildableObject::StaticClass(), Location, FRotator::ZeroRotator);
	Placed->SetMesh(CurrentGhostMesh);
}


void ABuildManager::UpdateGhost() const
{
	if(!CurrentGhost) return;

	FVector HitLocation;
	if(!GetCursorHit(HitLocation)) return;

	FVector MeshExtent = CurrentGhost->GetMeshExtent(); 
	FVector GhostLocation = HitLocation + FVector(0.f, 0.f, MeshExtent.Z); // pivot not in the center anymore 
	
	FVector Snapped = GhostLocation;
	Snapped.X = FMath::RoundToFloat(Snapped.X / SnapSize) * SnapSize;
	Snapped.Y = FMath::RoundToFloat(Snapped.Y / SnapSize) * SnapSize;

	CurrentGhost->SetActorLocation(Snapped);
	
	bool bValid = CanPlace(Snapped, CurrentGhost->GetMeshExtent());
	CurrentGhost->SetValid(bValid);
}

bool ABuildManager::CanPlace(const FVector& Location, const FVector& Extent) const
{
	FCollisionShape BoxShape = FCollisionShape::MakeBox(Extent);
	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	
	FCollisionObjectQueryParams ObjectQuery;
	ObjectQuery.AddObjectTypesToQuery(ECC_GameTraceChannel1); // Build

	bool bBlocked = GetWorld()->OverlapAnyTestByObjectType(
		Location,
		FQuat::Identity,
		ObjectQuery,
		BoxShape,
		Params
	);

	
	return !bBlocked;
}

bool ABuildManager::GetCursorHit(FVector& OutHit) const
{
	if(APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		float MouseX, MouseY;
		if(PC->GetMousePosition(MouseX, MouseY))
		{
			FVector WorldOrigin, WorldDir;
			if(PC->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldOrigin, WorldDir))
			{
				FHitResult Hit;
				if(GetWorld()->LineTraceSingleByChannel(Hit, WorldOrigin, WorldOrigin + WorldDir * 10000.f, ECC_Visibility))
				{
					OutHit = Hit.Location;
					return true;
				}
			}
		}
	}
	return false;
}

