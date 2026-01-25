#include "AI/BTTask_TryToEat.h"
#include "AI/NPC.h"
#include "AI/NPCController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerController.h"
#include "Player/CrowdedPlayerState.h"

UBTTask_TryToEat::UBTTask_TryToEat(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Try To Eat";
}

EBTNodeResult::Type UBTTask_TryToEat::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UFoodComponent* NPCFoodComponent = nullptr;
	UFoodComponent* PlayerFoodComponent = nullptr;
	
	// Get NPC Food component
	ANPCController* const Controller = Cast<ANPCController>(OwnerComp.GetAIOwner());
	if (!Controller)
		return EBTNodeResult::Failed;

	
	ANPC* const NPC = Cast<ANPC>(Controller->GetPawn());
	if (!NPC)
		return EBTNodeResult::Failed;

	NPCFoodComponent = NPC->GetFoodComponent();
	
	// Get Player Food Component
	ACrowdedPlayerController* PlayerController = Cast<ACrowdedPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PlayerController)
		return EBTNodeResult::Failed;

	ACrowdedPlayerState* PlayerState = PlayerController->GetPlayerState<ACrowdedPlayerState>();
	if (!PlayerState)
		return EBTNodeResult::Failed;

	PlayerFoodComponent = PlayerState->GetFoodComponent();
	
	if (!NPCFoodComponent || !PlayerFoodComponent)
		return EBTNodeResult::Failed;
	
	int NeededFood = 100 - NPCFoodComponent->GetFood();
	if (PlayerFoodComponent->HasEnoughFood(NeededFood))
	{
		NPCFoodComponent->AddFood(NeededFood);
		PlayerFoodComponent->RemoveFood(NeededFood);
		return EBTNodeResult::Succeeded;
	}
	
	// todo: check si pas assez de food dans player pour pas ajouter dans npc

	return EBTNodeResult::Failed;
}
