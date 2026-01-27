#include "AI/BTDecorators/BTDecorator_IsEating.h"

#include "AIController.h"
#include "Resources/FoodComponent.h"

UBTDecorator_IsEating::UBTDecorator_IsEating(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Is Eating";

	bCreateNodeInstance = true; // Chaque NPC a sa propre instance
	bNotifyTick = true;
}

bool UBTDecorator_IsEating::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return false;

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
		return false;

	UFoodComponent* FoodComp = Pawn->FindComponentByClass<UFoodComponent>();
	if (!FoodComp)
		return false;
	
	return FoodComp->GetIsEating();
}

void UBTDecorator_IsEating::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const bool bCurrentValue = CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	if (bCurrentValue != CachedValue)
	{
		CachedValue = bCurrentValue;
		OwnerComp.RequestExecution(this); 
	}
}
