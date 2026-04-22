#include "Action/Actions/Action_Fire_Extinguish.h"

UAction_Fire_Extinguish::UAction_Fire_Extinguish()
{
	ActionName = "Extinguish";
}

bool UAction_Fire_Extinguish::CanExecute_Implementation(AActor* Instigator) const
{
	return Super::CanExecute_Implementation(Instigator);
}

void UAction_Fire_Extinguish::Execute_Implementation(AActor* Instigator)
{
	Super::Execute_Implementation(Instigator);
}
