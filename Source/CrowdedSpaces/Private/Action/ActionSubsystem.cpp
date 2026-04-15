#include "Action/ActionSubsystem.h"

#include "AI/NPC.h"

void UActionSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
}

TStatId UActionSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UBuildSubsystem, STATGROUP_Tickables);
}

void UActionSubsystem::SelectNPC(ANPC* NPC)
{
	if (SelectedNPC == NPC)
	{
		DeselectNPC();
		return;
	}
	
	if (SelectedNPC != nullptr)
		DeselectNPC();
	
	// Outline
	if (OutlineMaterial != nullptr)
		NPC->GetMesh()->SetOverlayMaterial(OutlineMaterial);

	SelectedNPC = NPC;
}

void UActionSubsystem::DeselectNPC()
{
	if (SelectedNPC == nullptr)
		return;

	// Outline
	SelectedNPC->GetMesh()->SetOverlayMaterial(nullptr);
	
	SelectedNPC = nullptr;
}

void UActionSubsystem::OnSelectableActorSelected(AActor* SelectedActor)
{
	ANPC* NewSelectedNPC = Cast<ANPC>(SelectedActor);
	
	if (NewSelectedNPC != nullptr)
		SelectNPC(NewSelectedNPC);
	else
		DeselectNPC();
}

void UActionSubsystem::OnNotSelectableActorSelected()
{
	DeselectNPC(); // a voir si on garde pas tjrs un npc selected ? 
}
