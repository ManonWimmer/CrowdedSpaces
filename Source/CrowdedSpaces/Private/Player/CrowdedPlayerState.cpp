#include "Player/CrowdedPlayerState.h"

ACrowdedPlayerState::ACrowdedPlayerState()
{
	const TObjectPtr<UResourceComponent> MoneyComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("MoneyComponent"));
	MoneyComponent->SetType(EResourceType::Money);
	MoneyComponent->SetCanLoseAndRegenResource(false);
	ResourceMap.Add(EResourceType::Money, MoneyComponent);

	const TObjectPtr<UResourceComponent> ElectricityComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("ElectricityComponent"));
	ElectricityComponent->SetType(EResourceType::Electricity);
	ElectricityComponent->SetCanLoseAndRegenResource(false);
	ResourceMap.Add(EResourceType::Electricity, ElectricityComponent);

	const TObjectPtr<UResourceComponent> FoodComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("FoodComponent"));
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

int ACrowdedPlayerState::GetResourceByType(const EResourceType Type) const
{
	if (!GetResourceComponentByType(Type))
		return 0;
	else
		return GetResourceComponentByType(Type)->GetResource();
}
