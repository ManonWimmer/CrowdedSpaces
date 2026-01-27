#include "MoralEvent/Events/MoralEvent_ExpelNPC.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Player/CrowdedPlayerController.h"
#include "Player/CrowdedPlayerState.h"
#include "Production/ProductionType.h"

class ACrowdedPlayerController;

void UMoralEvent_ExpelNPC::ClickOnChoice(EMoralEventChoice Choice)
{
	Super::ClickOnChoice(Choice);
	
	switch (Choice)
	{
		case EMoralEventChoice::ExpelNPC:
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Yellow, "Decided to do expel npc.");

			// Get NPCs in world
			UWorld* World = GetWorld();
			if (!World)
				break;

			TArray<ANPC*> FoundNPCs;
		
			for (TActorIterator<ANPC> It(World); It; ++It) 
			{
				ANPC* NPC = *It;
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

			// Get random npc
			int32 RandomIndex = FMath::RandRange(0, FoundNPCs.Num() - 1);
			ANPC* RandomNPC = FoundNPCs[RandomIndex];

			// Destroy it
			RandomNPC->Destroy();

			// Add player money
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (!PC)
				break;

			ACrowdedPlayerController* CamPC = Cast<ACrowdedPlayerController>(PC);
			if (!CamPC)
				break;

			UMoneyComponent* MoneyComponent = nullptr;
		
			if (ACrowdedPlayerState* PS = PC->GetPlayerState<ACrowdedPlayerState>())
			{
				MoneyComponent = PS->GetMoneyComponent();
			}

			if (!MoneyComponent)
				break;

			MoneyComponent->AddMoney(MoneyForExpel);
		
			break;
		}
	
		case EMoralEventChoice::DoNothing: 
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

	Choices = {EMoralEventChoice::ExpelNPC, EMoralEventChoice::DoNothing};
}
