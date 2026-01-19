#include "Build/BuildManager.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Build/BuildableObject.h"
#include "Components/WidgetComponent.h"
#include "Player/CrowdedPlayerController.h"
#include "Game/GameModeSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerState.h"

ABuildManager::ABuildManager(): CurrentGhost(nullptr), CurrentBuildData(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABuildManager::BeginPlay()
{
	Super::BeginPlay();

	// Game Mode
	if (UGameModeSubsystem* Mode = GetWorld()->GetSubsystem<UGameModeSubsystem>())
	{
		Mode->OnGameModeChanged.AddDynamic(this, &ABuildManager::OnGameModeChanged);
	}

	// Player controller
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (ACrowdedPlayerController* CamPC = Cast<ACrowdedPlayerController>(PC))
		{
			CamPC->OnLeftClickBuild.AddDynamic(this, &ABuildManager::PlaceObject);

			// Money component
			if (ACrowdedPlayerState* PS = PC->GetPlayerState<ACrowdedPlayerState>())
			{
				MoneyComponent = PS->GetMoneyComponent();
			}
		}
	}

	// HUD
	GameHUD = Cast<AGameHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void ABuildManager::OnGameModeChanged(EGameModeState NewMode)
{
	// Activer ou désactiver le tick selon le mode
	SetActorTickEnabled(NewMode == EGameModeState::Building);

	StopBuilding();

	// UI
	if (NewMode == EGameModeState::Building)
	{
		if (GameHUD) GameHUD->ShowBuildWidget(true);
	}
	else
	{
		if (GameHUD) GameHUD->ShowBuildWidget(false);
	}
}

void ABuildManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateGhost();
}

void ABuildManager::StartBuilding(UBuildData* BuildData)
{
	if (!BuildData || !BuildData->BuildClass) return;

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
	const ABuildableObject* DefaultBuildable =
		BuildData->BuildClass->GetDefaultObject<ABuildableObject>();

	if (!DefaultBuildable) return;
	
	UStaticMesh* GhostMesh = DefaultBuildable->GetMeshComponent()->GetStaticMesh();
	CurrentGhost->SetMesh(GhostMesh);

	// Scale
	CurrentGhost->SetActorScale3D(DefaultBuildable->GetActorScale3D());
}

void ABuildManager::StopBuilding()
{
	CurrentBuildData = nullptr;

	if (CurrentGhost)
	{
		CurrentGhost->SetActorHiddenInGame(true);
	}
}


bool IsCursorOverUI(UWorld* World)
{
	TArray<UUserWidget*> Widgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, Widgets, UUserWidget::StaticClass(), true);

	for (UUserWidget* Widget : Widgets)
	{
		if (Widget && Widget->IsInViewport() && Widget->IsHovered())
		{
			return true; // le curseur est sur un widget UI
		}
	}

	return false;
}

void ABuildManager::PlaceObject()
{
	if (!CurrentGhost || !CurrentBuildData || !CurrentBuildData->BuildClass)
		return;

	// todo : check if click on ui & return if true
	
	const FVector Location = CurrentGhost->GetActorLocation();
	const FVector Extent = CurrentGhost->GetMeshExtent();

	if (!CanPlace(Location, Extent))
		return;

	ABuildableObject* Placed = GetWorld()->SpawnActor<ABuildableObject>(
		CurrentBuildData->BuildClass,
		Location,
		FRotator::ZeroRotator
	);

	if (!Placed) return;

	//  Scale from BP
	const ABuildableObject* DefaultBuildable =
		CurrentBuildData->BuildClass->GetDefaultObject<ABuildableObject>();

	Placed->SetActorScale3D(DefaultBuildable->GetActorScale3D());

	// Money
	if (MoneyComponent)
	{
		MoneyComponent->RemoveMoney(CurrentBuildData->MoneyCost);
	}
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

