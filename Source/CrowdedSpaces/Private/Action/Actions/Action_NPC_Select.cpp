#include "Action/Actions/Action_NPC_Select.h"

UAction_NPC_Select::UAction_NPC_Select()
{
	ActionName = "Select";
}

bool UAction_NPC_Select::CanExecute_Implementation(AActor* Instigator) const
{
	return Super::CanExecute_Implementation(Instigator);
}

void UAction_NPC_Select::Execute_Implementation(AActor* Instigator)
{
	Super::Execute_Implementation(Instigator);
}
