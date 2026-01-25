#include "Build/BuildableGenerator.h"

ABuildableGenerator::ABuildableGenerator()
{
	ProductionComponent = CreateDefaultSubobject<UProductionComponent>("ProductionComponent");
}

void ABuildableGenerator::BeginPlay()
{
	Super::BeginPlay();
	
	ProductionComponent->StartProduction();
}

#pragma region Selectable
void ABuildableGenerator::OnSelected()
{
}

void ABuildableGenerator::OnDeselected()
{
}

FString ABuildableGenerator::GetDisplayName() const
{
	return "Generator";
}

AActor* ABuildableGenerator::GetSelectableActor()
{
	return this;
}
#pragma endregion Selectable

