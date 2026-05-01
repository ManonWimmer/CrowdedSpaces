#include "Fire/Fire.h"

#include "Action/Action.h"
#include "Action/ActionComponent.h"
#include "Components/SphereComponent.h"
#include "Game/CrowdedGameState.h"

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
}

void AFire::BeginPlay()
{
	Super::BeginPlay();
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

