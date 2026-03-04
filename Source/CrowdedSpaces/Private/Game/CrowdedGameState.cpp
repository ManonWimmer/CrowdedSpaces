#include "Game/CrowdedGameState.h"

#include "AI/NameGeneratorSubsystem.h"
#include "Build/BuildSubsystem.h"
#include "MoralEvent/MoralEventSubsystem.h"
#include "Time/TimeSubsystem.h"

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

	// Get name generator subsystem & send data
	const TObjectPtr<UNameGeneratorSubsystem> NameGeneratorSubsystem = GetWorld()->GetSubsystem<UNameGeneratorSubsystem>();
	if (!NameGeneratorSubsystem)
		return;

	NameGeneratorSubsystem->SetNameData(NameData);

	// Get build subsystem & send data
	const TObjectPtr<UBuildSubsystem> BuildSubsystem = GetWorld()->GetSubsystem<UBuildSubsystem>();
	if (!BuildSubsystem)
		return;
		
	BuildSubsystem->SetBuildData(BuildDataObjects);
	BuildSubsystem->SetBuildRoomData(BuildDataRooms);
	BuildSubsystem->SetSnapSize(SnapSize);

	// Get time subsystem & send data
	const TObjectPtr<UTimeSubsystem> TimeSubsystem = GetWorld()->GetSubsystem<UTimeSubsystem>();
	if (!TimeSubsystem)
		return;

	TimeSubsystem->SetTimeData(TimeData);

	// Get moral event subsystem & send data
	const TObjectPtr<UMoralEventSubsystem> MoralEventSubsystem = GetWorld()->GetSubsystem<UMoralEventSubsystem>();
	if (!MoralEventSubsystem)
		return;

	MoralEventSubsystem->SetPossibleEvents(PossibleMoralEvents);
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
