#include "Game/CrowdedGameState.h"

#include "Action/ActionSubsystem.h"
#include "AI/NameGeneratorSubsystem.h"
#include "Build/BuildSubsystem.h"
#include "MoralEvent/MoralEventSubsystem.h"
#include "Storage/StorageSubsystem.h"
#include "Time/TimeSubsystem.h"
#include "Training/TrainingSubsystem.h"

ACrowdedGameState::ACrowdedGameState()
{
	// Resources
	MoneyComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("MoneyComponent"));
	MoneyComponent->SetType(EResourceType::Money);
	MoneyComponent->SetCanLoseAndRegenResource(false);
	ResourceMap.Add(EResourceType::Money, MoneyComponent);

	ElectricityComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("ElectricityComponent"));
	ElectricityComponent->SetType(EResourceType::Electricity);
	ElectricityComponent->SetCanLoseAndRegenResource(false);
	ResourceMap.Add(EResourceType::Electricity, ElectricityComponent);

	FoodComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("FoodComponent"));
	FoodComponent->SetType(EResourceType::Food);
	FoodComponent->SetCanLoseAndRegenResource(false);
	ResourceMap.Add(EResourceType::Food, FoodComponent);
}

void ACrowdedGameState::BeginPlay()
{
	Super::BeginPlay();

	TryInitSubsystems();
}

void ACrowdedGameState::TryInitSubsystems()
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to init subsystems..."));
	
	// Get name generator subsystem & send data
	const TObjectPtr<UNameGeneratorSubsystem> NameGeneratorSubsystem = GetWorld()->GetSubsystem<UNameGeneratorSubsystem>();
	if (!NameGeneratorSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Name generator subsystem not found"));
		return;
	}

	NameGeneratorSubsystem->SetNameData(NameData);

	// Get build subsystem & send data
	const TObjectPtr<UBuildSubsystem> BuildSubsystem = GetWorld()->GetSubsystem<UBuildSubsystem>();
	if (!BuildSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Build subsystem not found"));
		return;
	}

	BuildSubsystem->SetBuildRoomData(BuildDataRooms);
	BuildSubsystem->SetBuildData(BuildDataObjects);
	BuildSubsystem->SetSnapSize(SnapSize);

	// Get time subsystem & send data
	const TObjectPtr<UTimeSubsystem> TimeSubsystem = GetWorld()->GetSubsystem<UTimeSubsystem>();
	if (!TimeSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Time subsystem not found"));
		return;
	}

	TimeSubsystem->SetTimeData(TimeData);

	// Get moral event subsystem & send data
	const TObjectPtr<UMoralEventSubsystem> MoralEventSubsystem = GetWorld()->GetSubsystem<UMoralEventSubsystem>();
	if (!MoralEventSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Moral event subsystem not found"));
		return;
	}

	MoralEventSubsystem->SetPossibleEvents(PossibleMoralEvents);

	// Get storage subsystem & send data
	const TObjectPtr<UStorageSubsystem> StorageSubsystem = GetWorld()->GetSubsystem<UStorageSubsystem>();
	if (!StorageSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Storage subsystem not found"));
		return;
	}

	StorageSubsystem->SetStorageData(StorageData);

	// Get training subsystem & send data
	const TObjectPtr<UTrainingSubsystem> TrainingSubsystem = GetWorld()->GetSubsystem<UTrainingSubsystem>();
	if (!TrainingSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Training subsystem not found"));
		return;
	}

	TrainingSubsystem->SetTrainingData(TrainingData);

	// Get action subsystem & send data
	const TObjectPtr<UActionSubsystem> ActionSubsystem = GetWorld()->GetSubsystem<UActionSubsystem>();
	if (!ActionSubsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Action subsystem not found"));
		return;
	}

	ActionSubsystem->SetOutlineMaterial(OutlineMaterial);

	UE_LOG(LogTemp, Warning, TEXT("All subsystem found!"));

	bHasInitSubsystems = true;
	OnGameDataReady.Broadcast();
}

UResourceComponent* ACrowdedGameState::GetResourceComponentByType(EResourceType Type) const
{
	if (const TObjectPtr<UResourceComponent>* Found = ResourceMap.Find(Type))
	{
		return Found->Get();
	}

	return nullptr;
}

int ACrowdedGameState::GetResourceByType(EResourceType Type) const
{
	if (!GetResourceComponentByType(Type))
		return 0;
	else
		return GetResourceComponentByType(Type)->GetResource();
}
