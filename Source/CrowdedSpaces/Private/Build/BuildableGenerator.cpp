#include "Build/BuildableGenerator.h"

ABuildableGenerator::ABuildableGenerator()
{
	ProductionComponent = CreateDefaultSubobject<UProductionComponent>("ProductionComponent");
}

void ABuildableGenerator::SetNPCWorking(bool bWorking)
{
	bHasNPCWorking = bWorking;

	if (bHasNPCWorking)
		ProductionComponent->ResumeOrStartProduction();
	else
		ProductionComponent->PauseProduction();
}

void ABuildableGenerator::BeginPlay()
{
	Super::BeginPlay();
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

