#include "Action/Actions/Action_HealingStation_Heal.h"

#include "Object/Buildable/BuildableHealingStation.h"

UAction_HealingStation_Heal::UAction_HealingStation_Heal()
{
	ActionName = "Heal";
}

bool UAction_HealingStation_Heal::CanExecute_Implementation(AActor* Instigator) const
{
	const TObjectPtr<ABuildableHealingStation> HealingStation = Cast<ABuildableHealingStation>(Instigator);
	if (!HealingStation)
		return false;

	const TObjectPtr<ANPC> SelectedNPC = ActionSubsystem->GetSelectedNPC();
	if (!SelectedNPC)
		return false;

	if (!HealingStation->CanBeUsed())
		return false;

	return HealingStation->IsAvailableForReservation(SelectedNPC);
}

void UAction_HealingStation_Heal::Execute_Implementation(AActor* Instigator)
{
	Super::Execute_Implementation(Instigator);

	const TObjectPtr<ABuildableHealingStation> HealingStation = Cast<ABuildableHealingStation>(Instigator);
	if (!HealingStation)
		return;
	
	if (!ActionSubsystem)
		return;

	const TObjectPtr<ANPC> SelectedNPC = ActionSubsystem->GetSelectedNPC();
	if (!SelectedNPC)
		return;
	
	SelectedNPC->SetActionObject(HealingStation);
}
