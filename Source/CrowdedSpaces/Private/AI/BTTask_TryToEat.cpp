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

	// Get NPC Food Component
	if (ANPCController* const Controller = Cast<ANPCController>(OwnerComp.GetAIOwner()))
	{
		if (ANPC* const NPC = Cast<ANPC>(Controller->GetPawn()))
		{
			NPCFoodComponent = NPC->GetFoodComponent();
		}
	}

	// todo: mettre en begin play
	// Get Player Food Component
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		if (ACrowdedPlayerController* PlayerController = Cast<ACrowdedPlayerController>(PC))
		{
			if (ACrowdedPlayerState* PS = PlayerController->GetPlayerState<ACrowdedPlayerState>())
			{
				PlayerFoodComponent = PS->GetFoodComponent();
			}
		}
	}
	
	if (!NPCFoodComponent || !PlayerFoodComponent)
		return EBTNodeResult::Failed;
	
	int NeededFood = 100 - NPCFoodComponent->GetFood();
	if (PlayerFoodComponent->HasEnoughFood(NeededFood))
	{
		NPCFoodComponent->AddFood(NeededFood);
		PlayerFoodComponent->RemoveFood(NeededFood);
		return EBTNodeResult::Succeeded;
	}
	// todo: check si pas assed de food dans player pour pas ajouter dans npc

	return EBTNodeResult::Failed;
}
