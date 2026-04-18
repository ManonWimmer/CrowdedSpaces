#include "Action/Actions/Action_NPC_Select.h"

#include "AI/NPC.h"

UAction_NPC_Select::UAction_NPC_Select()
{
	ActionName = "Select";
}

bool UAction_NPC_Select::CanExecute_Implementation(AActor* Instigator) const
{
	const ANPC* NPC = Cast<ANPC>(Instigator);
	if (!NPC)
		return false;
	
	if (!ActionSubsystem)
		return false;
	
	if (ActionSubsystem->GetSelectedNPC() == NPC)
		return false;

	return true;
}

void UAction_NPC_Select::Execute_Implementation(AActor* Instigator)
{
	Super::Execute_Implementation(Instigator);

	ANPC* NPC = Cast<ANPC>(Instigator);
	if (!NPC)
		return;
	
	if (!ActionSubsystem)
		return;

	ActionSubsystem->SelectNPC(NPC);
}
