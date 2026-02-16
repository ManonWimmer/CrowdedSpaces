#include "Resources/ResourceComponent.h"

UResourceComponent::UResourceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UResourceComponent::SetType(EResourceType NewType)
{
	ResourceType = NewType;
	
	// Set default values
}

void UResourceComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (CanLoseAndRegenResource)
		StartResourceTimer();
}

void UResourceComponent::AddResource(int Amount)
{
	const int32 OldResource = Resource;

	Resource = FMath::Clamp(Resource + Amount, 0, MaxResource);

	if (Resource != OldResource)
	{
		OnResourceChanged.Broadcast(Resource);
	}

	if (Resource >= MaxResource)
	{
		OnResourceFull.Broadcast();
	}
}

void UResourceComponent::RemoveResource(int Amount)
{
	Resource = FMath::Clamp(Resource - Amount, 0, MaxResource);
	
	if (Resource <= 0)
		OnNoMoreResource.Broadcast();
	else
		OnResourceChanged.Broadcast(Resource);
}

bool UResourceComponent::HasEnoughResource(int Amount)
{
	return Resource >= Amount;
}

bool UResourceComponent::HasMaxResource()
{
	return Resource >= MaxResource;
}

void UResourceComponent::StartResourceTimer()
{
	if (!GetWorld()) return;

	GetWorld()->GetTimerManager().SetTimer(
		ResourceTimerHandle,
		this,
		&UResourceComponent::ResourceTick,
		TickInterval,
		true
	);
}

void UResourceComponent::StopResourceTimer()
{
	if (!GetWorld()) return;
	GetWorld()->GetTimerManager().ClearTimer(ResourceTimerHandle);
}

void UResourceComponent::SetIsInRegen(bool bInRegen)
{
	bIsInRegen = bInRegen;
	OnIsInRegenChanged.Broadcast(bIsInRegen);
}

void UResourceComponent::ResourceTick()
{
	if (bIsInRegen)
	{
		AddResource(ResourceRegenPerTick);
	}
	else
	{
		RemoveResource(ResourceLossPerTick);
	}
}

