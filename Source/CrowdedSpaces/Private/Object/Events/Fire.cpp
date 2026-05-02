#include "Object/Events/Fire.h"

#include "Action/Action.h"
#include "Action/ActionComponent.h"
#include "Components/SphereComponent.h"
#include "Game/CrowdedGameState.h"
#include "Damage/Damageable.h"
#include "Debug/CrowdedSpacesLogs.h"

AFire::AFire()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;

	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionObjectType(ECC_WorldDynamic);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SelectionType = ESelectionType::Default;
	ObjectType = EObjectType::Fire;
	NPCUseAction = ENPCActionType::ExtinguishFire;
	NPCMoveAction = ENPCActionType::MoveToFire;
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AFire::OnOverlapBegin);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AFire::OnOverlapEnd);
}
void AFire::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AFire::ApplyDamage,DamageInterval,true);
}

void AFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	CS_LOG("Fire progress : %f", GetFireExtinguishProgress());
	OnFireExtinguishProgress.Broadcast();

	if (CurrentExtinguishProgress >= TimeToExtinguish)
	{
		ExtinguishFire();
	}
}

#pragma region Fire
void AFire::ExtinguishFire()
{
	CS_LOG("Extinguish fire");
	OnFireExtinguished.Broadcast(this);
	
	Destroy();
}

void AFire::ApplyDamage()
{
	for (int i = OverlappingActors.Num() - 1; i >= 0; --i)
	{
		AActor* Actor = OverlappingActors[i];

		if (!Actor || !Actor->Implements<UDamageable>())
		{
			OverlappingActors.RemoveAt(i);
			continue;
		}

		IDamageable::Execute_TakeDamage(Actor, DamageAmount);
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

	if (OtherActor->Implements<UDamageable>())
	{
		OverlappingActors.AddUnique(OtherActor);
	}
}

void AFire::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (!OtherActor) return;

	OverlappingActors.Remove(OtherActor);
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

