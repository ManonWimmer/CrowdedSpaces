#include "Resources/ResourceComponent.h"

#include "Game/CrowdedGameInstance.h"
#include "Resources/ResourceDefaultsData.h"

UResourceComponent::UResourceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UResourceComponent::SetType(EResourceType NewType)
{
	ResourceType = NewType;

	// Set default values
	static UResourceDefaultsData* DefaultsData = nullptr;
	if (!DefaultsData)
	{
		ConstructorHelpers::FObjectFinder<UResourceDefaultsData> Finder(TEXT("/Game/Project/Data/Resources/DA_ResourceDefaults.DA_ResourceDefaults"));
		if (Finder.Succeeded())
		{
			DefaultsData = Finder.Object;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ResourceDefaultsData not found!"));
			return;
		}
	}
	
	if (DefaultsData && DefaultsData->DefaultsByType.Contains(ResourceType))
	{
		const FResourceDefaults& Defaults = DefaultsData->DefaultsByType[ResourceType];
		MaxResource = Defaults.MaxResource;
		Resource = MaxResource;
		CanLoseAndRegenResource = Defaults.CanLoseAndRegenResource;
		ResourceLossPerTick = Defaults.ResourceLossPerTick;
		ResourceRegenPerTick = Defaults.ResourceRegenPerTick;
		TickInterval = Defaults.TickInterval;
	}
}

void UResourceComponent::BeginPlay()
{
	Super::BeginPlay();


	const UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("World is null!"));
		return;
	}
	
	GameInstance = World->GetGameInstance<UCrowdedGameInstance>();

	if (!GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstance is null!"));
		return;
	}
	
	if (CanLoseAndRegenResource)
		StartResourceTimer();
}

void UResourceComponent::AddResource(const int Amount)
{
	const int32 OldResource = Resource;

	if (MaxResource != -1)
		Resource = FMath::Clamp(Resource + Amount, 0, MaxResource);
	else
		Resource += Amount;

	if (Resource != OldResource)
	{
		OnResourceChanged.Broadcast(Resource);
	}

	if (Resource >= MaxResource && MaxResource != -1)
	{
		OnResourceFull.Broadcast();
	}
}

void UResourceComponent::RemoveResource(const int Amount)
{
	// Check game instance can lose food/energy
	if ((ResourceType == EResourceType::Food && !GameInstance->bNPCsCanLoseFood) ||
		(ResourceType == EResourceType::Energy && !GameInstance->bNPCsCanLoseEnergy))
		return;
	
	if (MaxResource != -1)
		Resource = FMath::Clamp(Resource - Amount, 0, MaxResource);
	else
		Resource -= Amount;
	
	if (Resource <= 0)
		OnNoMoreResource.Broadcast();
	
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
	if (!GetWorld())
		return;

	GetWorld()->GetTimerManager().SetTimer(
		ResourceTimerHandle,
		this,
		&UResourceComponent::ResourceTick,
		TickInterval,
		true
	);
}

void UResourceComponent::ToggleResourceTimer()
{
	if (!GetWorld())
		return;

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();

	if (TimerManager.IsTimerActive(ResourceTimerHandle))
	{
		TimerManager.PauseTimer(ResourceTimerHandle);
	}
	else if (TimerManager.IsTimerPaused(ResourceTimerHandle))
	{
		TimerManager.UnPauseTimer(ResourceTimerHandle);
	}
	else
	{
		StartResourceTimer();
	}
}

void UResourceComponent::StopResourceTimer()
{
	if (!GetWorld())
		return;
	GetWorld()->GetTimerManager().ClearTimer(ResourceTimerHandle);
}

void UResourceComponent::SetCanLoseAndRegenResource(bool bCanLoseAndRegen)
{
	CanLoseAndRegenResource = bCanLoseAndRegen;
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

