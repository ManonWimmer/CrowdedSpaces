#include "AI/BTDecorators/BTDecorator_IsHungry.h"
#include "AIController.h"

#include "Resources/ResourceComponent.h"

UBTDecorator_IsHungry::UBTDecorator_IsHungry(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Is Hungry";

	bCreateNodeInstance = true; // Chaque NPC a sa propre instance
	bNotifyTick = true;
}

bool UBTDecorator_IsHungry::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	TObjectPtr<AAIController> AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return false;

	TObjectPtr<APawn> Pawn = AIController->GetPawn();
	if (!Pawn)
		return false;

	TObjectPtr<UResourceComponent> FoodComp = nullptr;
	TArray<UResourceComponent*> ResourceComps;
	Pawn->GetComponents(ResourceComps);
	for (UResourceComponent* Comp : ResourceComps)
	{
		if (Comp && Comp->GetType() == EResourceType::Food)
		{
			FoodComp = Comp;
			break;
		}
	}
	
	return FoodComp->GetResource() <= HungryUnderFood;
}

void UBTDecorator_IsHungry::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const bool bCurrentValue = CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	if (bCurrentValue != CachedValue)
	{
		CachedValue = bCurrentValue;
		OwnerComp.RequestExecution(this); 
	}
}
