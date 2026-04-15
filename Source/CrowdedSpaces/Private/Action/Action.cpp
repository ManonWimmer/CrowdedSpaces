#include "Action/Action.h"

bool UAction::CanExecute_Implementation(AActor* Instigator) const
{
	return true;
}

void UAction::Execute_Implementation(AActor* Instigator)
{
}
