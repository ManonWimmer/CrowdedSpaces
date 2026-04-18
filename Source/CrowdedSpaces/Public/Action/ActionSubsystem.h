#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ActionSubsystem.generated.h"

class UActionWidgetManager;
class ACrowdedGameMode;
class ANPC;

UCLASS()
class CROWDEDSPACES_API UActionSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual TStatId GetStatId() const override;
	virtual void Tick(float DeltaTime) override;

	// NPC
	void SelectFirstPossibleNPC();
	void SelectNPC(ANPC* NPC);
	void DeselectNPC();

	void ShowActionsForActor(AActor* Actor);
	void HideActions();

	UFUNCTION()
	void OnNPCUnregistered(ANPC* UnregisteredNPC);
	
	UFUNCTION()
	void OnNPCRegistered(ANPC* RegisteredNPC, bool bWasFirst);
	
	// Outline
	void SetOutlineMaterial(UMaterialInterface* NewOutlineMaterial);
	
private:
	UPROPERTY()
	TObjectPtr<ANPC> SelectedNPC{nullptr};

	UPROPERTY()
	TObjectPtr<UMaterialInterface> OutlineMaterial{nullptr}; // Sent by game state

	UPROPERTY()
	TObjectPtr<ACrowdedGameMode> GameMode{nullptr};

	UPROPERTY()
	TObjectPtr<UActionWidgetManager> ActionWidgetManager{nullptr};
};
