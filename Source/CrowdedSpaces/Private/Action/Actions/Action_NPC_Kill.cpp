#include "Action/Actions/Action_NPC_Kill.h"

#include "AI/NPC.h"

UAction_NPC_Kill::UAction_NPC_Kill()
{
	ActionName = "Kill";
}

bool UAction_NPC_Kill::CanExecute_Implementation(AActor* Instigator) const
{
	return Super::CanExecute_Implementation(Instigator);
}

void UAction_NPC_Kill::Execute_Implementation(AActor* Instigator)
{
	Super::Execute_Implementation(Instigator);

	ANPC* NPC = Cast<ANPC>(Instigator);
	if (!NPC)
		return;

	NPC->Die();
}
