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
	
	TMap<int, FGridRoom>& Rooms = BuildSubsystem->GetRooms();

	// Rooms
	for (auto& Pair : Rooms)
	{
		FGridRoom& Room = Pair.Value;
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
			if (FGridRoom* RoomPtr = Rooms.Find(Object->GetBuildData()->RoomId))
			{
				if (!(RoomPtr)->bHasEnoughElectricity)
				{
					Object->SetHasEnoughElectricity(false);
					continue;
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
