#include "Storage/StorageSubsystem.h"

#include "Game/CrowdedGameState.h"
#include "Resources/ResourceComponent.h"
#include "Build/BuildSubsystem.h"
#include "Storage/StorageData.h"

void UStorageSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	UWorld* World = InWorld.GetWorld();
	if (!World)
		return;
	
	const TObjectPtr<ACrowdedGameState> GameState = World->GetGameState<ACrowdedGameState>();
	if (!GameState)
		return;

	MoneyComponent = GameState->GetResourceComponent<EResourceType::Money>();
	FoodComponent = GameState->GetResourceComponent<EResourceType::Food>();
	ElectricityComponent = GameState->GetResourceComponent<EResourceType::Electricity>();
	
	BuildSubsystem = InWorld.GetSubsystem<UBuildSubsystem>();
	BuildSubsystem->OnRoomDestroyed.AddDynamic(this, &UStorageSubsystem::OnRoomDestroyed);
	BuildSubsystem->OnRoomCreated.AddDynamic(this, &UStorageSubsystem::OnRoomCreated);
	BuildSubsystem->OnRoomUpdated.AddDynamic(this, &UStorageSubsystem::OnRoomUpdated);
}

TStatId UStorageSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UBuildSubsystem, STATGROUP_Tickables);
}

void UStorageSubsystem::OnRoomCreated(const int RoomId, const EGridRoomType RoomType)
{
	if (RoomType != EGridRoomType::Storage)
		return;

	AddStorageRoom(RoomId);
}

void UStorageSubsystem::OnRoomUpdated(const int RoomId, const EGridRoomType RoomType)
{
	if (RoomType != EGridRoomType::Storage)
		return;

	UpdateStorageRoom(RoomId);
}

void UStorageSubsystem::AddStorageRoom(const int RoomId)
{
	UE_LOG(LogTemp, Display, TEXT("Adding storage room %d"), RoomId);

	int RoomCellsCount = BuildSubsystem->GetRoomCellsCount(RoomId);
	
	FStorageRoomValues StorageRoomValues;
	
	StorageRoomValues.AddMaxMoneyPerCell = StorageData->StorageRoomAddMaxMoneyPerCell;
	StorageRoomValues.AddMaxMoneyTotal = StorageData->StorageRoomAddMaxMoneyPerCell * RoomCellsCount;
	
	StorageRoomValues.AddMaxFoodPerCell = StorageData->StorageRoomAddMaxFoodPerCell;
	StorageRoomValues.AddMaxFoodTotal = StorageData->StorageRoomAddMaxFoodPerCell * RoomCellsCount;
	
	StorageRoomValues.AddMaxElectricityPerCell = StorageData->StorageRoomAddMaxElectricityPerCell;
	StorageRoomValues.AddMaxElectricityTotal = StorageData->StorageRoomAddMaxElectricityPerCell * RoomCellsCount;

	MoneyComponent->AddMaxResource(StorageRoomValues.AddMaxMoneyTotal);
	FoodComponent->AddMaxResource(StorageRoomValues.AddMaxFoodTotal);
	ElectricityComponent->AddMaxResource(StorageRoomValues.AddMaxElectricityTotal);
	
	StorageRooms.Add(RoomId, StorageRoomValues);
}

void UStorageSubsystem::UpdateStorageRoom(const int RoomId)
{
	int RoomCellsCount = BuildSubsystem->GetRoomCellsCount(RoomId);
	
	FStorageRoomValues StorageRoomValues = StorageRooms[RoomId];
	FStorageRoomValues NewStorageRoomValues = StorageRoomValues;

	// Money
	float NewAddMaxMoneyTotal = StorageData->StorageRoomAddMaxMoneyPerCell * RoomCellsCount;
	if (NewAddMaxMoneyTotal != StorageRoomValues.AddMaxMoneyTotal)
	{
		float AddMaxMoneyOffset = NewAddMaxMoneyTotal - StorageRoomValues.AddMaxMoneyTotal;
		if (AddMaxMoneyOffset > 0)
		{
			MoneyComponent->AddMaxResource(AddMaxMoneyOffset);
		}
		else
		{
			MoneyComponent->RemoveMaxResource(abs(AddMaxMoneyOffset));
		}
	}
	NewStorageRoomValues.AddMaxMoneyTotal = NewAddMaxMoneyTotal;

	// Food
	float NewAddMaxFoodTotal = StorageData->StorageRoomAddMaxFoodPerCell * RoomCellsCount;
	if (NewAddMaxFoodTotal != StorageRoomValues.AddMaxFoodTotal)
	{
		float AddMaxFoodOffset = NewAddMaxFoodTotal - StorageRoomValues.AddMaxFoodTotal;
		if (AddMaxFoodOffset > 0)
		{
			FoodComponent->AddMaxResource(AddMaxFoodOffset);
		}
		else
		{
			FoodComponent->RemoveMaxResource(abs(AddMaxFoodOffset));
		}
	}
	NewStorageRoomValues.AddMaxFoodTotal = NewAddMaxFoodTotal;

	// Electricity
	float NewAddMaxElectricityTotal = StorageData->StorageRoomAddMaxElectricityPerCell * RoomCellsCount;
	if (NewAddMaxElectricityTotal != StorageRoomValues.AddMaxElectricityTotal)
	{
		float AddMaxElectricityOffset = NewAddMaxElectricityTotal - StorageRoomValues.AddMaxElectricityTotal;
		if (AddMaxElectricityOffset > 0)
		{
			ElectricityComponent->AddMaxResource(AddMaxElectricityOffset);
		}
		else
		{
			ElectricityComponent->RemoveMaxResource(abs(AddMaxElectricityOffset));
		}
	}
	NewStorageRoomValues.AddMaxElectricityTotal = NewAddMaxElectricityTotal;

	StorageRooms[RoomId] = NewStorageRoomValues;
}

void UStorageSubsystem::OnRoomDestroyed(const int RoomId)
{
	UE_LOG(LogTemp, Display, TEXT("Destroyed room %d"), RoomId);
	
	if (!StorageRooms.Find(RoomId))
		return;

	UE_LOG(LogTemp, Display, TEXT("Remove storage room %d"), RoomId);
	
	FStorageRoomValues StorageRoomValues = StorageRooms[RoomId];
	
	MoneyComponent->RemoveMaxResource(StorageRoomValues.AddMaxMoneyTotal);
	FoodComponent->RemoveMaxResource(StorageRoomValues.AddMaxFoodTotal);
	ElectricityComponent->RemoveMaxResource(StorageRoomValues.AddMaxElectricityTotal);
		
	StorageRooms.Remove(RoomId);
}

FStorageRoomValues UStorageSubsystem::GetStorageValuesForCreatedRoom(const int RoomId)
{
	return StorageRooms[RoomId];
}
