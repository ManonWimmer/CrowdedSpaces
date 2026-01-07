#include "Game/GameManager.h"
#include "EngineUtils.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraController.h"

AGameManager::AGameManager(): BuildManager(nullptr)
{
	CurrentMode = EGameModeState::Building; // temp test, plus tard game
}

void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
	for (TActorIterator<ACameraController> It(GetWorld()); It; ++It)
	{
		ACameraController* CamPC = *It;
		if (!CamPC) continue;

		CamPC->OnLeftClick.AddDynamic(this, &AGameManager::OnPlayerClick);
	}
}

void AGameManager::OnPlayerClick()
{
	if(CurrentMode != EGameModeState::Building || !BuildManager) return;

	FVector HitLocation;
	if(GetPlayerClickLocation())
	{
		BuildManager->PlaceObject();
	}
}

bool AGameManager::GetPlayerClickLocation() const
{
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		// Ignore si souris sur UI
		if(UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld()).IsZero())
		{
			return false;
		}

		return true;
	}
	return false;
}
