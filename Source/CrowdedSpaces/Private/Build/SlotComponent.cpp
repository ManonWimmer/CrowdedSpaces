#include "Build/SlotComponent.h"
#include "AI/NPC.h"

USlotComponent::USlotComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void USlotComponent::BeginPlay()
{
	Super::BeginPlay();

	MID = CreateDynamicMaterialInstance(0);

	UpdateVisual();
}

void USlotComponent::SetOccupied(bool bNewOccupied, ANPC* NPC)
{
	bIsOccupied = bNewOccupied;
	OccupyingNPC = bNewOccupied ? NPC : nullptr;

	UpdateVisual();
}

void USlotComponent::UpdateVisual()
{
	if (!MID) return;

	if (bIsOccupied)
	{
		MID->SetVectorParameterValue("Color", FLinearColor::Red);
	}
	else
	{
		MID->SetVectorParameterValue("Color", FLinearColor::Green);
	}
}

