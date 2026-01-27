#include "AI/BTDecorators/BTDecorator_IsSleeping.h"

#include "AIController.h"
#include "Resources/EnergyComponent.h"

UBTDecorator_IsSleeping::UBTDecorator_IsSleeping(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Is Sleeping";

	bCreateNodeInstance = true; // Chaque NPC a sa propre instance
	bNotifyTick = true;
}

bool UBTDecorator_IsSleeping::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return false;

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
		return false;

	UEnergyComponent* EnergyComp = Pawn->FindComponentByClass<UEnergyComponent>();
	if (!EnergyComp)
		return false;
	
	return EnergyComp->GetIsSleeping();
}

void UBTDecorator_IsSleeping::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const bool bCurrentValue = CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	if (bCurrentValue != CachedValue)
	{
		CachedValue = bCurrentValue;
		OwnerComp.RequestExecution(this); 
	}
}
