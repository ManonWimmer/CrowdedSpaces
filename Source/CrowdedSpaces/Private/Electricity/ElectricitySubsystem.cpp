#include "Electricity/ElectricitySubsystem.h"

#include "Build/BuildableRegistrySubsystem.h"
#include "Build/BuildData.h"
#include "Game/CrowdedGameState.h"

void UElectricitySubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	GameState = InWorld.GetGameState<ACrowdedGameState>();
	if (!GameState)
		return;

	ElectricityComponent = GameState->GetResourceComponent<EResourceType::Electricity>();
}

TStatId UElectricitySubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UElectricitySubsystem, STATGROUP_Tickables);
}

void UElectricitySubsystem::OnTimeChanged(const float NewTime)
{
	int CurrentHour = NewTime / 60;
	if (CurrentHour != LastHour)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "New hour");

		LastHour = CurrentHour;
		
		UWorld* World = GetWorld();
		if (!World)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "no world");
			return;
		}
		
		TObjectPtr<UBuildableRegistrySubsystem> BRS = World->GetSubsystem<UBuildableRegistrySubsystem>();
		if (!BRS)
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "no brs");
			return;
		}
		
		// Objects
		for (TWeakObjectPtr<ABuildableObject> Object : BRS->BuildableObjects) 
		{
			if (!Object.IsValid())
				continue;

			int ObjectLoseElectricity = Object->GetBuildData()->LoseElectricityPerHour;
			if (ObjectLoseElectricity == 0)
				continue;

			if (ElectricityComponent->HasEnoughResource(ObjectLoseElectricity))
			{
				ElectricityComponent->RemoveResource(ObjectLoseElectricity);
				// remettre can work
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "can work");
			}
			else
			{
				// cant work
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "cant work");
			}
		}
	}
	
}
