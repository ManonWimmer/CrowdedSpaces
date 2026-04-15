#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ActionSubsystem.generated.h"

class ANPC;

UCLASS()
class CROWDEDSPACES_API UActionSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual TStatId GetStatId() const override;

	// NPC
	void SelectNPC(ANPC* NPC);
	void DeselectNPC();

	// Selection
	UFUNCTION()
	void OnSelectableActorSelected(AActor* SelectedActor);

	UFUNCTION()
	void OnNotSelectableActorSelected();
	
	// Outline
	void SetOutlineMaterial(UMaterialInterface* NewOutlineMaterial) { OutlineMaterial = NewOutlineMaterial; }
	
private:
	UPROPERTY()
	TObjectPtr<ANPC> SelectedNPC{nullptr};

	UPROPERTY()
	TObjectPtr<UMaterialInterface> OutlineMaterial{nullptr}; // Sent by game state
};
