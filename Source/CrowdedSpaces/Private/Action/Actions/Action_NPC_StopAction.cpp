#include "Action/Actions/Action_NPC_StopAction.h"

UAction_NPC_StopAction::UAction_NPC_StopAction()
{
	ActionName = "Stop Action";
}

bool UAction_NPC_StopAction::CanExecute_Implementation(AActor* Instigator) const
{
	return Super::CanExecute_Implementation(Instigator);
}

void UAction_NPC_StopAction::Execute_Implementation(AActor* Instigator)
{
	Super::Execute_Implementation(Instigator);

	// current work nullptr
	// current train nullptr
}
