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
	
	UBuildSubsystem* BuildSubsystem = InWorld.GetSubsystem<UBuildSubsystem>();
	BuildSubsystem->OnRoomDestroyed.AddDynamic(this, &UStorageSubsystem::OnRoomDestroyed);
	BuildSubsystem->OnRoomCreated.AddDynamic(this, &UStorageSubsystem::OnRoomCreated);
	// todo: on room updated (aggrandir)
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

void UStorageSubsystem::AddStorageRoom(const int RoomId)
{
	// Struct pour plus tard avoir des updrades de storage room pour avoir des max + grands
	// ou pour avoir des values * nbr cells de la room

	UE_LOG(LogTemp, Display, TEXT("Adding storage room %d"), RoomId);
	
	FStorageRoomValues StorageRoomValues;
	StorageRoomValues.AddMaxMoney = StorageData->StorageRoomAddMaxMoney;
	StorageRoomValues.AddMaxFood = StorageData->StorageRoomAddMaxFood;
	StorageRoomValues.AddMaxElectricity = StorageData->StorageRoomAddMaxElectricity;

	MoneyComponent->AddMaxResource(StorageRoomValues.AddMaxMoney);
	FoodComponent->AddMaxResource(StorageRoomValues.AddMaxFood);
	ElectricityComponent->AddMaxResource(StorageRoomValues.AddMaxElectricity);
	
	StorageRooms.Add(RoomId, StorageRoomValues);
}

void UStorageSubsystem::OnRoomDestroyed(const int RoomId)
{
	UE_LOG(LogTemp, Display, TEXT("Destroyed room %d"), RoomId);
	
	if (!StorageRooms.Find(RoomId))
		return;

	UE_LOG(LogTemp, Display, TEXT("Remove storage room %d"), RoomId);
	
	FStorageRoomValues StorageRoomValues = StorageRooms[RoomId];
	
	MoneyComponent->RemoveMaxResource(StorageRoomValues.AddMaxMoney);
	FoodComponent->RemoveMaxResource(StorageRoomValues.AddMaxFood);
	ElectricityComponent->RemoveMaxResource(StorageRoomValues.AddMaxElectricity);
		
	StorageRooms.Remove(RoomId);
}
