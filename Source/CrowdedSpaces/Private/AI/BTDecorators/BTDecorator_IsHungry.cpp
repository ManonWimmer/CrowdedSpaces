#include "AI/BTDecorators/BTDecorator_IsHungry.h"

#include "AIController.h"
#include "Resources/FoodComponent.h"

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

	TObjectPtr<UFoodComponent> FoodComp = Pawn->FindComponentByClass<UFoodComponent>();
	if (!FoodComp)
		return false;
	
	return FoodComp->GetFood() <= HungryUnderFood;
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
