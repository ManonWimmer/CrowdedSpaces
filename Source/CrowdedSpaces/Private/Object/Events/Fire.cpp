#include "Object/Events/Fire.h"

#include "Action/Action.h"
#include "Action/ActionComponent.h"
#include "Components/SphereComponent.h"
#include "Game/CrowdedGameState.h"
#include "Damage/Damageable.h"
#include "Debug/CrowdedSpacesLogs.h"
#include "UI/UIUtils.h"
#include "UI/Widgets/World/FireProgressWidget.h"

AFire::AFire()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;

	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionObjectType(ECC_WorldDynamic);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	ClickCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ClickCollision"));
	ClickCollision->SetupAttachment(RootComponent);

	ClickCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ClickCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	ClickCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	ClickCollision->SetSphereRadius(50.f);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SelectionType = ESelectionType::Default;
	ObjectType = EObjectType::Fire;
	
	NPCUseAction = ENPCActionType::ExtinguishFire;
	NPCMoveAction = ENPCActionType::MoveToFire;
	
	UsingResourceTypeToCheck = EResourceType::None;
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AFire::OnOverlapBegin);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AFire::OnOverlapEnd);

	// Progress widget
	ExtinguishProgressWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ExtinguishProgressWidget"));
	ExtinguishProgressWidget->SetupAttachment(RootComponent);
}
void AFire::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AFire::ApplyDamage,DamageInterval,true);

	PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return;
	
	// Setup extinguish progress widget
	if (!ExtinguishProgressWidget)
		return;

	const TObjectPtr<UUserWidget> ExtinguishProgressUserWidget = ExtinguishProgressWidget->GetUserWidgetObject();
	if (!ExtinguishProgressUserWidget)
		return;

	const TObjectPtr<UFireProgressWidget> ExtinguishProgressWidgetPtr = Cast<UFireProgressWidget>(ExtinguishProgressUserWidget);
	if (!ExtinguishProgressWidgetPtr)
		return;
	
	ExtinguishProgressWidgetPtr->OwningActor = this;
	ExtinguishProgressWidgetPtr->Init();
}

void AFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FUIUtils::RotateComponentToCameraYaw(PlayerController, ExtinguishProgressWidget);
	
	if (NPCsExtinguishing.Num() == 0)
	{
		// Remove progress on NPC stop extinguish
		if (CurrentExtinguishProgress > 0)
		{
			CurrentExtinguishProgress -= DeltaTime;
			
			if (CurrentExtinguishProgress <= 0)
				CurrentExtinguishProgress = 0;
			
			OnFireExtinguishProgress.Broadcast();
		}
		
		return;
	}
	
	NPCsExtinguishing.RemoveAll([](const TWeakObjectPtr<ANPC>& NPC)
	{
		return !NPC.IsValid();
	});

	const int NPCCount = NPCsExtinguishing.Num();

	CurrentExtinguishProgress += DeltaTime * NPCCount;
	//CS_LOG("Fire progress : %f", GetFireExtinguishProgress());
	OnFireExtinguishProgress.Broadcast();

	if (CurrentExtinguishProgress >= TimeToExtinguish)
	{
		ExtinguishFire();
	}
}

#pragma region Fire
void AFire::ExtinguishFire()
{
	//CS_LOG("Extinguish fire");
	OnFireExtinguished.Broadcast(this);
	
	Destroy();
}

void AFire::ApplyDamage()
{
	//CS_LOG("ApplyDamage tick | Components count = %d", DamageableComponents.Num());
	
	for (int i = DamageableComponents.Num() - 1; i >= 0; --i)
	{
		if (!DamageableComponents[i])
		{
			//CS_LOG_WARNING("Invalid component at index %d", i);
			DamageableComponents.RemoveAt(i);
			continue;
		}

		//CS_LOG("Applying damage to comp ptr=%p", DamageableComponents[i]);
		
		DamageableComponents[i]->RemoveResource(DamageAmount);
	}
}

float AFire::GetFireExtinguishProgress() const
{
	return CurrentExtinguishProgress / TimeToExtinguish;
}
#pragma endregion

#pragma region Overlap Take Damage
void AFire::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	//CS_LOG("OverlapBegin: Actor = %s", *OtherActor->GetName())
	TArray<UResourceComponent*> ResourceComps;
	OtherActor->GetComponents<UResourceComponent>(ResourceComps);

	bool bFoundHealth = false;

	for (UResourceComponent* Comp : ResourceComps)
	{
		if (!Comp)
			continue;

		//CS_LOG("Found ResourceComponent | Actor=%s | Type=%d | ptr=%p", *OtherActor->GetName(),(int32)Comp->GetType(),Comp);

		if (Comp->GetType() == EResourceType::Health)
		{
			//CS_LOG("HEALTH COMPONENT ADDED");
			DamageableComponents.AddUnique(Comp);
			bFoundHealth = true;
		}
	}

	if (!bFoundHealth)
	{
		CS_LOG_WARNING("No HealthComponent found on %s", *OtherActor->GetName());
	}
}

void AFire::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (!OtherActor) return;

	//CS_LOG("OverlapEnd: Actor = %s", *OtherActor->GetName());
	
	TArray<UResourceComponent*> ResourceComps;
	OtherActor->GetComponents<UResourceComponent>(ResourceComps);

	for (UResourceComponent* Comp : ResourceComps)
	{
		if (!Comp)
			continue;

		if (Comp->GetType() == EResourceType::Health)
		{
			//CS_LOG("Removing HealthComponent | Actor=%s | ptr=%p", *OtherActor->GetName(), Comp);

			DamageableComponents.Remove(Comp);
		}
	}
}
#pragma endregion

#pragma region Using Object
bool AFire::StartUsingImplementation(ANPC* NPC)
{
	if (!NPC)
		return false;

	NPCsExtinguishing.AddUnique(NPC);
	return true;
}

bool AFire::StopUsingImplementation(ANPC* NPC)
{
	NPCsExtinguishing.Remove(NPC);
	return true;
}
#pragma endregion 

#pragma region Actions
void AFire::InitActions()
{
	if (!GameState)
		return;
	
	TArray<TObjectPtr<UAction>> InstancedActions;

	for (const TSubclassOf<UAction>& ActionClass : GameState->FireActions)
	{
		if (!ActionClass) continue;

		UAction* NewAction = NewObject<UAction>(this, ActionClass);
		if (!NewAction) continue;

		NewAction->Initialize(GetWorld());
		InstancedActions.Add(NewAction);
	}

	ActionComponent->SetupActions(InstancedActions);
}
#pragma endregion

#pragma region Selection
void AFire::OnSelected()
{
}

void AFire::OnDeselected()
{
}
#pragma endregion

