#include "Fire/Fire.h"

#include "Action/Action.h"
#include "Action/ActionComponent.h"
#include "Components/SphereComponent.h"
#include "Game/CrowdedGameState.h"
#include "Damage/Damageable.h"

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
	NPCAction = ENPCActionType::ExtinguishFire;
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AFire::OnOverlapBegin);
	SphereCollision->OnComponentEndOverlap.AddDynamic(this, &AFire::OnOverlapEnd);
}

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



void AFire::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(DamageTimerHandle, this, &AFire::ApplyDamage,DamageInterval,true);
}

void AFire::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

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

#pragma region Fire
void AFire::ExtinguishFire()
{
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
#pragma endregion

#pragma region Selection
void AFire::OnSelected()
{
}

void AFire::OnDeselected()
{
}

bool AFire::StartUsingImplementation(ANPC* NPC)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Fire");

	return true;
}

bool AFire::StopUsingImplementation(ANPC* NPC)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Stop Fire");

	return true;
}
#pragma endregion

