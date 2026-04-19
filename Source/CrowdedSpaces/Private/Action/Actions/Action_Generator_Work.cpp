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

	// set current generator : instigator si cast ok
	// set current train nullptr

	ABuildableGenerator* Generator = Cast<ABuildableGenerator>(Instigator);
	if (!Generator)
		return;

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Found generator as instigator");

	if (!ActionSubsystem)
		return;
	
	ANPC* SelectedNPC = ActionSubsystem->GetSelectedNPC();
	if (!SelectedNPC)
		return;

	USlotComponent* Slot = Generator->GetNearestFreeSlot(SelectedNPC->GetActorLocation());
	if (!Slot)
		return;
	
	AAIController* AIController = Cast<AAIController>(SelectedNPC->GetController());
	if (AIController)
	{
		UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
		if (Blackboard)
		{
			Blackboard->SetValueAsObject("Generator", Generator);
			Blackboard->SetValueAsObject("GeneratorSlot", Slot);
			Blackboard->SetValueAsVector("GeneratorLocation", Slot->GetComponentLocation());
			Blackboard->SetValueAsObject("TrainStation", nullptr);
			Blackboard->SetValueAsObject("TrainStationSlot", nullptr);
			Blackboard->SetValueAsVector("TrainStationLocation", FVector::Zero());
		}
	}

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Work complete execute");
}
