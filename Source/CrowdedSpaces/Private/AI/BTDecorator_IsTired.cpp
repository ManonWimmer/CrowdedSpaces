#include "AI/BTDecorator_IsTired.h"

#include "AIController.h"
#include "Resources/EnergyComponent.h"

UBTDecorator_IsTired::UBTDecorator_IsTired(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Is Tired";
}

bool UBTDecorator_IsTired::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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
	
	return EnergyComp->GetEnergy() <= TiredUnderEnergy;
}
