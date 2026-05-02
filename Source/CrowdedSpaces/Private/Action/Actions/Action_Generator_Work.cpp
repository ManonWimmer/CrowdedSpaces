#include "Action/Actions/Action_Generator_Work.h"

#include "AI/NPC.h"
#include "Object/Buildable/BuildableGenerator.h"

UAction_Generator_Work::UAction_Generator_Work()
{
	ActionName = "Work";
}

bool UAction_Generator_Work::CanExecute_Implementation(AActor* Instigator) const
{
	const ABuildableGenerator* Generator = Cast<ABuildableGenerator>(Instigator);
	if (!Generator)
		return false;

	const ANPC* SelectedNPC = ActionSubsystem->GetSelectedNPC();
	if (!SelectedNPC)
		return false;

	if (!Generator->CanBeUsed())
		return false;

	return Generator->IsAvailableForReservation(SelectedNPC);
}

void UAction_Generator_Work::Execute_Implementation(AActor* Instigator)
{
	Super::Execute_Implementation(Instigator);
	
	ABuildableGenerator* Generator = Cast<ABuildableGenerator>(Instigator);
	if (!Generator)
		return;
	
	if (!ActionSubsystem)
		return;

	ANPC* SelectedNPC = ActionSubsystem->GetSelectedNPC();
	if (!SelectedNPC)
		return;
	
	SelectedNPC->SetActionObject(Generator);
}
