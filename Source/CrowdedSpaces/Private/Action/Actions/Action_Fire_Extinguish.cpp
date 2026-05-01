#include "Action/Actions/Action_Fire_Extinguish.h"

#include "Fire/Fire.h"

UAction_Fire_Extinguish::UAction_Fire_Extinguish()
{
	ActionName = "Extinguish";
}

bool UAction_Fire_Extinguish::CanExecute_Implementation(AActor* Instigator) const
{
	const AFire* Fire = Cast<AFire>(Instigator);
	if (!Fire)
		return false;

	const ANPC* SelectedNPC = ActionSubsystem->GetSelectedNPC();
	if (!SelectedNPC)
		return false;

	return Fire->IsAvailableForReservation(SelectedNPC);
}

void UAction_Fire_Extinguish::Execute_Implementation(AActor* Instigator)
{
	Super::Execute_Implementation(Instigator);

	const TObjectPtr<AFire> Fire = Cast<AFire>(Instigator);
	if (!Fire)
		return;
	
	//Fire->ExtinguishFire();
	if (!ActionSubsystem)
		return;

	ANPC* SelectedNPC = ActionSubsystem->GetSelectedNPC();
	if (!SelectedNPC)
		return;
	
	SelectedNPC->SetActionObject(Fire);
}
