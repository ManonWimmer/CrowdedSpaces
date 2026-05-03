#include "Action/Actions/Action_NPC_StopAction.h"

#include "AI/NPC.h"

UAction_NPC_StopAction::UAction_NPC_StopAction()
{
	ActionName = "Stop Action";
}

bool UAction_NPC_StopAction::CanExecute_Implementation(AActor* Instigator) const
{
	const TObjectPtr<ANPC> NPC = Cast<ANPC>(Instigator);
	if (!NPC)
		return false;
	
	if (!NPC->HasActionObject())
		return false;

	return true;
}

void UAction_NPC_StopAction::Execute_Implementation(AActor* Instigator)
{
	Super::Execute_Implementation(Instigator);

	const TObjectPtr<ANPC> NPC = Cast<ANPC>(Instigator);
	if (!NPC)
		return;
	
	NPC->StopAction();
}
