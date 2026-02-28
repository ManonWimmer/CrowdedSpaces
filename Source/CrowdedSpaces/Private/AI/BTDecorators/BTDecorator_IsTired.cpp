#include "AI/BTDecorators/BTDecorator_IsTired.h"

#include "AIController.h"
#include "Resources/ResourceComponent.h"

UBTDecorator_IsTired::UBTDecorator_IsTired(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Is Tired";

	bCreateNodeInstance = true; // Chaque NPC a sa propre instance
	bNotifyTick = true;
}

bool UBTDecorator_IsTired::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	TObjectPtr<AAIController> AIController = OwnerComp.GetAIOwner();
	if (!AIController)
		return false;

	TObjectPtr<APawn> Pawn = AIController->GetPawn();
	if (!Pawn)
		return false;

	TObjectPtr<UResourceComponent> EnergyComp = nullptr;
	TArray<UResourceComponent*> ResourceComps;
	Pawn->GetComponents(ResourceComps);
	for (UResourceComponent* Comp : ResourceComps)
	{
		if (Comp && Comp->GetType() == EResourceType::Energy)
		{
			EnergyComp = Comp;
			break;
		}
	}
	
	return EnergyComp->GetResource() <= TiredUnderEnergy;
}

void UBTDecorator_IsTired::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const bool bCurrentValue = CalculateRawConditionValue(OwnerComp, NodeMemory);
	
	if (bCurrentValue != CachedValue)
	{
		CachedValue = bCurrentValue;
		OwnerComp.RequestExecution(this); 
	}
}
