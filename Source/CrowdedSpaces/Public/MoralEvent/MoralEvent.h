#pragma once

#include "CoreMinimal.h"
#include "MoralEventType.h"
#include "MoralEvent.generated.h"

enum class EResourceType : uint8;
class UResourceComponent;
class ACrowdedGameState;
class ANPC;

UCLASS(Blueprintable, BlueprintType)
class CROWDEDSPACES_API UMoralEvent : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void ClickOnChoice(EMoralEventType Choice);

	UFUNCTION()
	virtual void SetupChoices();

	AActor* SpawnNPC(const TSubclassOf<ANPC> NPCClass) const;
	void KillSelectedNPC() const;

	void AddPlayerResource(EResourceType ResourceType, int Amount) const;
	void RemovePlayerResource(EResourceType ResourceType, int Amount) const;

	UFUNCTION(BlueprintCallable)
	TArray<EMoralEventType> GetChoices() { return Choices; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MoralEvent")
	FString EventTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MoralEvent")
	FString EventDescription;

	UPROPERTY()
	TArray<EMoralEventType> Choices;

private:
	UPROPERTY()
	TObjectPtr<ACrowdedGameState> GameState = nullptr;

	UPROPERTY()
	TObjectPtr<ANPC> SelectedNPC = nullptr;
};
