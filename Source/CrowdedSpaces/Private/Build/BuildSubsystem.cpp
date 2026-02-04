#include "Build/BuildSubsystem.h"

#include "EngineUtils.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Game/CrowdedGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerController.h"
#include "Resources/MoneyComponent.h"
#include "Player/CrowdedPlayerState.h"

void UBuildSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	// Game Mode
	TObjectPtr<ACrowdedGameMode> GameMode = Cast<ACrowdedGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!GameMode)
		return;
	
	GameMode->OnGameModeChanged.AddDynamic(this, &UBuildSubsystem::OnGameModeChanged);

	// Player controller
	TObjectPtr<APlayerController> PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PC)
		return;

	TObjectPtr<ACrowdedPlayerController> CamPC = Cast<ACrowdedPlayerController>(PC);
	if (!CamPC)
		return;

	CamPC->OnLeftClickBuild.AddDynamic(this, &UBuildSubsystem::PlaceObject);

	// Money component
	if (TObjectPtr<ACrowdedPlayerState> PS = PC->GetPlayerState<ACrowdedPlayerState>())
	{
		MoneyComponent = PS->GetMoneyComponent();
	}

	// HUD
	GameHUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	// ----- TEMP TEST GRID ----- //
	for (TActorIterator<AGridActor> It(GetWorld()); It; ++It)
	{
		GridActor = *It;
		break; 
	}
	// ----- TEMP TEST GRID ----- //
}

TStatId UBuildSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UBuildSubsystem, STATGROUP_Tickables); // Sinon crash quand Tickable true
}

void UBuildSubsystem::OnGameModeChanged(EGameModeState NewMode)
{
	// Activer ou désactiver le tick selon le mode
	bTickEnabled = (NewMode == EGameModeState::Building);

	StopBuilding();

	// UI
	if (NewMode == EGameModeState::Building)
	{
		if (GameHUD)
			GameHUD->ShowBuildWidget(true);
	}
	else
	{
		if (GameHUD)
			GameHUD->ShowBuildWidget(false);
	}
}

void UBuildSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateGhost();
}

bool UBuildSubsystem::IsTickable() const
{
	return bTickEnabled;
}

void UBuildSubsystem::StartBuilding(UBuildData* BuildData)
{
	if (!BuildData || !BuildData->BuildClass)
		return;

	CurrentBuildData = BuildData;

	if (!CurrentGhost)
	{
		CurrentGhost = GetWorld()->SpawnActor<AGhostObject>(
			AGhostObject::StaticClass(),
			FVector::ZeroVector,
			FRotator::ZeroRotator
		);
	}

	CurrentGhost->SetActorHiddenInGame(false);

	// Get mesh from buildable
	TObjectPtr<ABuildableObject> const DefaultBuildable =
		BuildData->BuildClass->GetDefaultObject<ABuildableObject>();

	if (!DefaultBuildable)
		return;
	
	TObjectPtr<UStaticMesh> GhostMesh = DefaultBuildable->GetMeshComponent()->GetStaticMesh();
	CurrentGhost->SetMesh(GhostMesh);

	// Scale
	CurrentGhost->SetActorScale3D(DefaultBuildable->GetActorScale3D());
}

void UBuildSubsystem::StopBuilding()
{
	CurrentBuildData = nullptr;

	if (CurrentGhost)
		CurrentGhost->SetActorHiddenInGame(true);
}

void UBuildSubsystem::PlaceObject()
{
	if (!CurrentGhost || !CurrentBuildData || !CurrentBuildData->BuildClass)
		return;

	FVector GhostLocation = CurrentGhost->GetActorLocation();
	int Row, Column;
	if (!GridActor->GetCellAtLocation(GhostLocation, Row, Column))
		return;

	FGridCell* Cell = GridActor->GetGridCell(Row, Column);
	if (!Cell || Cell->bOccupied)
		return;

	TObjectPtr<ABuildableObject> Placed = GetWorld()->SpawnActor<ABuildableObject>(
		CurrentBuildData->BuildClass,
		GhostLocation,
		FRotator::ZeroRotator
	);

	if (!Placed)
		return;

	// Scale from default
	TObjectPtr<ABuildableObject> DefaultBuildable = CurrentBuildData->BuildClass->GetDefaultObject<ABuildableObject>();
	Placed->SetActorScale3D(DefaultBuildable->GetActorScale3D());
	
	Cell->bOccupied = true;
	
	if (MoneyComponent)
		MoneyComponent->RemoveMoney(CurrentBuildData->MoneyCost);

	/* OLD 
	const FVector Location = CurrentGhost->GetActorLocation();
	const FVector Extent = CurrentGhost->GetMeshExtent();

	if (!CanPlace(Location, Extent))
		return;

	TObjectPtr<ABuildableObject> Placed = GetWorld()->SpawnActor<ABuildableObject>(
		CurrentBuildData->BuildClass,
		Location,
		FRotator::ZeroRotator
	);

	if (!Placed)
		return;

	//  Scale from BP
	TObjectPtr<ABuildableObject> const DefaultBuildable =
		CurrentBuildData->BuildClass->GetDefaultObject<ABuildableObject>();

	Placed->SetActorScale3D(DefaultBuildable->GetActorScale3D());

	// Money
	if (MoneyComponent)
		MoneyComponent->RemoveMoney(CurrentBuildData->MoneyCost);
	*/
}


void UBuildSubsystem::UpdateGhost() const
{
	if(!CurrentGhost)
		return;

	FVector HitLocation;
	if(!GetCursorHit(HitLocation))
		return;

	int Row, Column;
	if (GridActor->GetCellAtLocation(HitLocation, Row, Column))
	{
		FVector2D CellLocation;
		GridActor->GetGridLocation(true, Row, Column, CellLocation);

		FVector SnappedLocation(CellLocation.X, CellLocation.Y, HitLocation.Z); 
		CurrentGhost->SetActorLocation(SnappedLocation);
		
		FGridCell* Cell = GridActor->GetGridCell(Row, Column);
		bool bValid = Cell && !Cell->bOccupied;
		CurrentGhost->SetValid(bValid);
	}

	/* OLD
	FVector MeshExtent = CurrentGhost->GetMeshExtent(); 
	FVector GhostLocation = HitLocation + FVector(0.f, 0.f, MeshExtent.Z); // pivot not in the center anymore 
	
	FVector Snapped = GhostLocation;
	Snapped.X = FMath::RoundToFloat(Snapped.X / SnapSize) * SnapSize;
	Snapped.Y = FMath::RoundToFloat(Snapped.Y / SnapSize) * SnapSize;

	CurrentGhost->SetActorLocation(Snapped);
	
	bool bValid = CanPlace(Snapped, CurrentGhost->GetMeshExtent());
	CurrentGhost->SetValid(bValid);
	*/
}

bool UBuildSubsystem::CanPlace(const FVector& Location, const FVector& Extent) const
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

bool UBuildSubsystem::GetCursorHit(FVector& OutHit) const
{
	TObjectPtr<APlayerController> PC = GetWorld()->GetFirstPlayerController();
	if (!PC)
		return false;
	
	float MouseX, MouseY;
	if(PC->GetMousePosition(MouseX, MouseY))
	{
		FVector WorldOrigin, WorldDir;
		if(PC->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldOrigin, WorldDir))
		{
			FHitResult Hit;
			if(GetWorld()->LineTraceSingleByChannel(Hit, WorldOrigin, WorldOrigin + WorldDir * CursorLineTraceDistance, ECC_Visibility))
			{
				OutHit = Hit.Location;
				return true;
			}
		}
	}
	return false;
}