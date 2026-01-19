#include "Game/CrowdedGameState.h"

#include "Build/BuildSubsystem.h"

void ACrowdedGameState::BeginPlay()
{
	Super::BeginPlay();

	// Get build subsystem & send data
	if (UBuildSubsystem* BuildSubsystem = GetWorld()->GetSubsystem<UBuildSubsystem>())
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Blue, "Set data of build subsystem");
		
		BuildSubsystem->SetBuildData(BuildDataObjects);
		BuildSubsystem->SetSnapSize(SnapSize);
	}
}
