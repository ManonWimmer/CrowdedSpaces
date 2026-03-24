#include "Electricity/ElectricitySubsystem.h"

#include "Build/BuildableRegistrySubsystem.h"
#include "Build/BuildSubsystem.h"
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
	float DeltaTime = NewTime - LastTime;
	LastTime = NewTime;

	if (DeltaTime <= 0.f)
		return;
	
	UWorld* World = GetWorld();
	if (!World)
		return;

	TObjectPtr<UBuildSubsystem> BuildSubsystem = World->GetSubsystem<UBuildSubsystem>();
	if (!BuildSubsystem)
		return;

	// Rooms
	for (TTuple<int, FGridRoom> Room : BuildSubsystem->GetRooms())
	{
		float LosePerHour = Room.Value.LoseElectricityPerHour;
		if (LosePerHour <= 0)
			continue;

		float ConsumptionThisFrame = (LosePerHour / 60.f) * DeltaTime;

		if (ElectricityComponent->HasEnoughResource(ConsumptionThisFrame))
		{
			ElectricityComponent->RemoveResource(ConsumptionThisFrame);
			if (!Room.Value.bHasEnoughElectricity)
			{
				Room.Value.bHasEnoughElectricity = true;
				OnRoomEnoughElectricityChanged.Broadcast(Room.Value.RoomId);
			}
		}
		else
		{
			if (Room.Value.bHasEnoughElectricity)
			{
				Room.Value.bHasEnoughElectricity = false;
				OnRoomEnoughElectricityChanged.Broadcast(Room.Value.RoomId);
			}
		}
	}

	TObjectPtr<UBuildableRegistrySubsystem> BRS = World->GetSubsystem<UBuildableRegistrySubsystem>();
	if (!BRS)
		return;
	
	// Objects
	for (TWeakObjectPtr<ABuildableObject> Object : BRS->BuildableObjects) 
	{
		if (!Object.IsValid())
			continue;

		float LosePerHour = Object->GetBuildData()->LoseElectricityPerHour;
		if (LosePerHour <= 0)
			continue;
		
		float ConsumptionThisFrame = (LosePerHour / 60.f) * DeltaTime;
		bool bIsObjectRoomActive = true;

		// Check if active room
		if (Object->GetBuildData()->RoomId != -1)
		{
			// a opti pour pas refaire la boucle à chaque fois, mettre dans une map ? 
			for (TTuple<int, FGridRoom> Room : BuildSubsystem->GetRooms())
			{
				if (Room.Value.RoomId == Object->GetBuildData()->RoomId)
				{
					if (!Room.Value.bHasEnoughElectricity)
					{
						Object->SetHasEnoughElectricity(false);
						bIsObjectRoomActive = false;
						continue;
					}
				}
			}
		}

		if (!bIsObjectRoomActive)
			continue;
		
		if (ElectricityComponent->HasEnoughResource(ConsumptionThisFrame))
		{
			ElectricityComponent->RemoveResource(ConsumptionThisFrame);
			Object->SetHasEnoughElectricity(true);
		}
		else
		{
			Object->SetHasEnoughElectricity(false);
		}
	}
}
