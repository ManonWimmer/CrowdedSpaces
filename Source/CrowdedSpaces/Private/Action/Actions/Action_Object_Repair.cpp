#include "Action/Actions/Action_Object_Repair.h"

UAction_Object_Repair::UAction_Object_Repair()
{
}

bool UAction_Object_Repair::CanExecute_Implementation(AActor* Instigator) const
{
	return Super::CanExecute_Implementation(Instigator);
}

void UAction_Object_Repair::Execute_Implementation(AActor* Instigator)
{
	Super::Execute_Implementation(Instigator);
}
