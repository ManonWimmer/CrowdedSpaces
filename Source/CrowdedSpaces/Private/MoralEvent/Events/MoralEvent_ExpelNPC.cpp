#include "MoralEvent/Events/MoralEvent_ExpelNPC.h"

#include "EngineUtils.h"
#include "Game/CrowdedGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerController.h"
#include "Player/CrowdedPlayerState.h"
#include "Production/ProductionType.h"

class ACrowdedPlayerController;

void UMoralEvent_ExpelNPC::ClickOnChoice(EMoralEventType Choice)
{
	Super::ClickOnChoice(Choice);
	
	switch (Choice)
	{
		case EMoralEventType::ExpelNPC:
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "Decided to do expel npc.");
				
			TObjectPtr<UWorld> World = GetWorld();
			if (!World)
				break;

			TArray<TObjectPtr<ANPC>> FoundNPCs;
		
			for (TActorIterator<ANPC> It(World); It; ++It) 
			{
				TObjectPtr<ANPC> NPC = *It;
				if (!NPC || !IsValid(NPC))
					continue;

				FoundNPCs.Add(NPC);
			}

			if (FoundNPCs.IsEmpty())
			{
				if (GEngine)
					GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Found 0 NPC in world.");
				break;
			}
				
			int32 RandomIndex = FMath::RandRange(0, FoundNPCs.Num() - 1);
			TObjectPtr<ANPC> RandomNPC = FoundNPCs[RandomIndex];
				
			RandomNPC->Destroy();

			TObjectPtr<UResourceComponent> MoneyComponent = nullptr;
		
			if (TObjectPtr<ACrowdedGameState> GameState = World->GetGameState<ACrowdedGameState>())
			{
				MoneyComponent = GameState->GetResourceComponent<EResourceType::Money>();
			}

			if (!MoneyComponent)
				break;

			MoneyComponent->AddResource(MoneyForExpel);
		
			break;
		}
	
		case EMoralEventType::DoNothing: 
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "Decided to do nothing.");
			break;
		
		default:
			break;
	}
}

void UMoralEvent_ExpelNPC::SetupChoices()
{
	Super::SetupChoices();

	Choices = {EMoralEventType::ExpelNPC, EMoralEventType::DoNothing};
}
