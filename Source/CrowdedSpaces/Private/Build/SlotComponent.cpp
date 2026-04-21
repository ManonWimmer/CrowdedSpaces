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

void USlotComponent::Acquire(ANPC* NPC)
{
	bIsOccupied = true;
	OccupyingNPC = NPC;

	UpdateVisual();
}

void USlotComponent::Release(ANPC* NPC)
{
	if (!OccupyingNPC || OccupyingNPC != NPC)
		return;
	
	bIsOccupied = false;
	OccupyingNPC = nullptr;

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

