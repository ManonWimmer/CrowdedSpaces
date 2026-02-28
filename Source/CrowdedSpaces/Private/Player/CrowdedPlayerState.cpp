#include "Player/CrowdedPlayerState.h"

ACrowdedPlayerState::ACrowdedPlayerState()
{
	MoneyComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("MoneyComponent"));
	MoneyComponent->SetType(EResourceType::Money);
	MoneyComponent->SetCanLoseAndRegenResource(false);
	ResourceMap.Add(EResourceType::Money, MoneyComponent);
	
	ElectricityComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("ElectricityComponent"));
	ElectricityComponent->SetType(EResourceType::Electricity);
	ElectricityComponent->SetCanLoseAndRegenResource(false);
	ResourceMap.Add(EResourceType::Electricity, ElectricityComponent);
	
	OxygenComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("OxygenComponent"));
	OxygenComponent->SetType(EResourceType::Oxygen);
	OxygenComponent->SetCanLoseAndRegenResource(false);
	ResourceMap.Add(EResourceType::Oxygen, OxygenComponent);
	
	FoodComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("FoodComponent"));
	FoodComponent->SetType(EResourceType::Food);
	FoodComponent->SetCanLoseAndRegenResource(false);
	ResourceMap.Add(EResourceType::Food, FoodComponent);
}

UResourceComponent* ACrowdedPlayerState::GetResourceComponentByType(const EResourceType Type) const
{
	if (const TObjectPtr<UResourceComponent>* Found = ResourceMap.Find(Type))
	{
		return Found->Get();
	}

	return nullptr;
}
