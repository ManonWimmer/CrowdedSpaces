#include "Action/ActionSubsystem.h"

#include "Action/ActionWidgetManager.h"
#include "AI/NPC.h"
#include "Game/CrowdedGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerController.h"

void UActionSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	const UWorld* World = InWorld.GetWorld();
	if (!World)
		return;
	
	GameMode = Cast<ACrowdedGameMode>(World->GetAuthGameMode());
	if (!GameMode)
		return;

	GameMode->OnNPCRegistered.AddDynamic(this, &UActionSubsystem::OnNPCRegistered);
	GameMode->OnNPCUnregistered.AddDynamic(this, &UActionSubsystem::OnNPCUnregistered);

	const TObjectPtr<APlayerController> PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!PlayerController)
		return;
	
	ActionWidgetManager = NewObject<UActionWidgetManager>(this);
	ActionWidgetManager->Initialize(PlayerController);
}

TStatId UActionSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UBuildSubsystem, STATGROUP_Tickables);
}

void UActionSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActionWidgetManager)
		ActionWidgetManager->Tick(DeltaTime);
}

void UActionSubsystem::SelectFirstPossibleNPC()
{
	for (ANPC* NPC : GameMode->GetAliveNPCs())
	{
		if (!NPC)
			continue;

		if (SelectedNPC && SelectedNPC == NPC)
			continue;

		SelectNPC(NPC);
		return;
	}
}

void UActionSubsystem::SelectNPC(ANPC* NPC)
{
	if (SelectedNPC && SelectedNPC == NPC)
	{
		DeselectNPC();
		return;
	}
	
	if (SelectedNPC != nullptr)
	{
		DeselectNPC();
	}
	
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

void UActionSubsystem::ShowActionsForActor(AActor* Actor)
{
	if (!ActionWidgetManager)
		return;

	ActionWidgetManager->ShowForActor(Actor);
}

void UActionSubsystem::HideActions()
{
	if (!ActionWidgetManager)
		return;

	ActionWidgetManager->Hide();
}

void UActionSubsystem::OnNPCUnregistered(ANPC* UnregisteredNPC)
{
	if (!UnregisteredNPC)
		return;
	
	if (SelectedNPC != UnregisteredNPC)
		return;

	SelectFirstPossibleNPC();

	if (!ActionWidgetManager)
		return;
	
	ActionWidgetManager->Hide();
}

void UActionSubsystem::OnNPCRegistered(ANPC* RegisteredNPC, bool bWasFirst)
{
	if (!RegisteredNPC)
		return;
	
	if (!bWasFirst)
		return;

	SelectFirstPossibleNPC();
}

void UActionSubsystem::SetOutlineMaterial(UMaterialInterface* NewOutlineMaterial)
{
	OutlineMaterial = NewOutlineMaterial;

	if (!SelectedNPC)
		return;

	SelectedNPC->GetMesh()->SetOverlayMaterial(OutlineMaterial);
	
}
