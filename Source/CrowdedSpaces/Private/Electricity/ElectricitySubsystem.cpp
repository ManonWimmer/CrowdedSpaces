#include "Electricity/ElectricitySubsystem.h"

#include "Build/BuildableRegistrySubsystem.h"
#include "Build/BuildSubsystem.h"
#include "Build/BuildData.h"
#include "Game/CrowdedGameState.h"
#include "Grid/GridRoom.h"

void UElectricitySubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	GameState = InWorld.GetGameState<ACrowdedGameState>();
	if (!GameState)
		return;

	ElectricityComponent = GameState->GetResourceComponent<EResourceType::Electricity>();
	
	BuildSubsystem = InWorld.GetSubsystem<UBuildSubsystem>();
	BuildableRegistrySubsystem = InWorld.GetSubsystem<UBuildableRegistrySubsystem>();
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
	
	TMap<int, FGridRoom>& Rooms = BuildSubsystem->GetRooms();

	// Rooms
	for (auto& Pair : Rooms)
	{
		FGridRoom& Room = Pair.Value;
		
		if (!Room.bIsActivated)
			continue;
			
		float LosePerHour = Room.LoseElectricityPerHour;
		if (LosePerHour <= 0)
			continue;

		float ConsumptionThisFrame = (LosePerHour / 60.f) * DeltaTime;

		bool bEnough = ElectricityComponent->HasEnoughResource(ConsumptionThisFrame);

		if (Room.bHasEnoughElectricity != bEnough)
		{
			Room.bHasEnoughElectricity = bEnough;
			OnRoomEnoughElectricityChanged.Broadcast(Room.RoomId);
		}

		if (bEnough)
		{
			ElectricityComponent->RemoveResource(ConsumptionThisFrame);
		}
	}
	
	// Objects
	for (TWeakObjectPtr<ABuildableObject> Object : BuildableRegistrySubsystem->BuildableObjects) 
	{
		if (!Object.IsValid())
			continue;

		if (!Object->IsActivated())
			continue;

		float LosePerHour = Object->GetBuildData()->LoseElectricityPerHour;
		if (LosePerHour <= 0)
			continue;
		
		float ConsumptionThisFrame = (LosePerHour / 60.f) * DeltaTime;

		// Check if active room
		if (Object->RoomId != BuildSubsystem->InvalidRoomId)
		{
			if (FGridRoom* RoomPtr = Rooms.Find(Object->RoomId))
			{
				if (!(RoomPtr)->bHasEnoughElectricity)
				{
					Object->SetHasEnoughElectricity(false);
					continue;
				}
			}
		}
		
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

void UElectricitySubsystem::ChangeRoomActiveState(int RoomId)
{
	TMap<int, FGridRoom>& Rooms = BuildSubsystem->GetRooms();
	FGridRoom* RoomPtr = Rooms.Find(RoomId);
	if (!RoomPtr)
		return;

	RoomPtr->bIsActivated = !RoomPtr->bIsActivated;

	// Activate / deactivate room objects
	for (TWeakObjectPtr<ABuildableObject> Object : BuildableRegistrySubsystem->BuildableObjects)
	{
		if (!Object.IsValid())
			continue;

		if (Object->RoomId != RoomId)
			continue;
		
		Object->SetIsActivated(RoomPtr->bIsActivated);
	}
}
