#include "Action/Actions/Action_Generator_Work.h"

#include "AIController.h"
#include "AI/NPC.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Build/SlotComponent.h"
#include "Build/Buildable/BuildableGenerator.h"

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

	return Generator->IsAvailableForReservation(SelectedNPC);
}

void UAction_Generator_Work::Execute_Implementation(AActor* Instigator)
{
	Super::Execute_Implementation(Instigator);

	// Set generator values in selected npc blackboard
	
	ABuildableGenerator* Generator = Cast<ABuildableGenerator>(Instigator);
	if (!Generator)
		return;
	
	if (!ActionSubsystem)
		return;
	
	ANPC* SelectedNPC = ActionSubsystem->GetSelectedNPC();
	if (!SelectedNPC)
		return;

	USlotComponent* Slot = Generator->GetNearestFreeSlot(SelectedNPC->GetActorLocation());
	if (!Slot)
		return;
	
	AAIController* AIController = Cast<AAIController>(SelectedNPC->GetController());
	if (!AIController)
		return;
	
	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard)
		return;

	Blackboard->SetValueAsObject("Generator", Generator);
	Blackboard->SetValueAsObject("GeneratorSlot", Slot);
	Blackboard->SetValueAsVector("GeneratorLocation", Slot->GetComponentLocation());
	
	Blackboard->SetValueAsObject("TrainStation", nullptr);
	Blackboard->SetValueAsObject("TrainStationSlot", nullptr);
	Blackboard->SetValueAsVector("TrainStationLocation", FVector::Zero());
}
