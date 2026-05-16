#include "Resources/ResourceComponent.h"

#include "Debug/CrowdedSpacesLogs.h"
#include "Game/CrowdedGameInstance.h"
#include "Game/CrowdedGameState.h"
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

	if (bSetMaxResourceAtStart)
		Resource = MaxResource;
	
	GameInstance = World->GetGameInstance<UCrowdedGameInstance>();

	if (!GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameInstance is null!"));
		return;
	}
	
	ACrowdedGameState* GameState = World->GetGameState<ACrowdedGameState>();
	if (!GameState)
		return;

	if (ResourceType == EResourceType::Food)
		PlayerFoodForRegen = GameState->GetResourceComponentByType(EResourceType::Food);
	
	StartResourceTimer();
}

void UResourceComponent::AddResource(const float Amount)
{
	CS_LOG("Add resource");
	const float OldResource = Resource;

	if (MaxResource != -1)
		Resource = FMath::Clamp(Resource + Amount, 0, MaxResource);
	else
		Resource += Amount;

	if (Resource != OldResource)
	{
		OnResourceAdded.Broadcast();
		OnResourceChanged.Broadcast(Resource);
	}

	if (Resource >= MaxResource && MaxResource != -1)
	{
		OnResourceFull.Broadcast();
	}
}

void UResourceComponent::RemoveResource(const float Amount)
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
	else
		OnResourceRemoved.Broadcast();
	
	OnResourceChanged.Broadcast(Resource);
}

void UResourceComponent::AddMaxResource(const float Amount)
{
	MaxResource += Amount;
	OnMaxResourceChanged.Broadcast(MaxResource);
}

void UResourceComponent::RemoveMaxResource(const float Amount)
{
	MaxResource -= Amount;
	Resource = FMath::Clamp(Resource, 0, MaxResource);

	OnMaxResourceChanged.Broadcast(MaxResource);
	OnResourceChanged.Broadcast(Resource);
}

bool UResourceComponent::HasEnoughResource(const float Amount)
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

void UResourceComponent::SetCanLoseAndRegenResource(const bool bCanLoseAndRegen)
{
	CanLoseAndRegenResource = bCanLoseAndRegen;
}

void UResourceComponent::SetIsInRegen(const bool bInRegen)
{
	bIsInRegen = bInRegen;
	check(!IsTemplate());

	//CS_LOG("OWNER VALID CHECK: %s | IsTemplate=%d | World=%s", *GetNameSafe(this), IsTemplate(), GetWorld() ? TEXT("VALID") : TEXT("NULL"));
	//CS_LOG("SET REGEN %s | ptr=%p | owner=%s", bIsInRegen ? TEXT("TRUE") : TEXT("FALSE"), this, *GetOwner()->GetName());
	OnIsInRegenChanged.Broadcast(bIsInRegen);
}

void UResourceComponent::ResourceTick()
{
	//CS_LOG("TICK %s | REGEN ACTUAL=%s | ptr=%p | owner=%s", *StaticEnum<EResourceType>()->GetValueAsString(ResourceType), bIsInRegen ? TEXT("TRUE") : TEXT("FALSE"), this, *GetOwner()->GetName());
	
	if (!bIsInRegen && !CanLoseAndRegenResource)
		return;
	
	//CS_LOG("resource tick type : %s", *StaticEnum<EResourceType>()->GetValueAsString(ResourceType));
	
	if (bIsInRegen)
	{
		//CS_LOG("is in regen");
		if (ResourceType == EResourceType::Food && PlayerFoodForRegen)
		{
			if (PlayerFoodForRegen->HasEnoughResource(ResourceRegenPerTick))
			{
				AddResource(ResourceRegenPerTick);
				PlayerFoodForRegen->RemoveResource(ResourceRegenPerTick);
			}
		}
		else
		{
			//CS_LOG("else add resource");
			AddResource(ResourceRegenPerTick);
		}
	}
	else if (CanLoseAndRegenResource)
	{
		RemoveResource(ResourceLossPerTick);
	}
}

