#pragma once

#include "CoreMinimal.h"
#include "Build/BuildSubsystem.h"
#include "Object/ObjectType.h"
#include "AI/NPC.h"
#include "Action/Action.h"
#include "Action/ActionComponent.h"
#include "GameFramework/Actor.h"
#include "UsableObject.generated.h"

class UBuildableRegistrySubsystem;
class AGameHUD;
class ACrowdedGameState;
class ACrowdedPlayerController;
class UBuildData;
class UBuildSubsystem;
class USlotComponent;
class UActionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlotsUpdated);

UCLASS()
class CROWDEDSPACES_API AUsableObject : public AActor
{
	GENERATED_BODY()

public:
	AUsableObject();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void SetMesh(UStaticMesh* Mesh) const;

	UFUNCTION()
	FVector GetExtent() const;

	UFUNCTION()
	UStaticMeshComponent* GetMeshComponent() const { return MeshComp; }

	UFUNCTION(BlueprintCallable, Category = "Object")
	bool CanBeUsed() const;

	// Slots
	USlotComponent* GetNearestFreeAndWalkableSlot(ANPC* NPC, const FVector& FromLocation);
	bool IsAvailableForReservation(const ANPC* NPC) const;
	void Release(ANPC* NPC);
	USlotComponent* ReserveSpecificSlot(ANPC* NPC, USlotComponent* Slot);
	void ReleaseSlot(ANPC* NPC);
	
	bool IsOverlappingCells(const TSet<FIntPoint>& Cells) const;

	UPROPERTY(BlueprintAssignable)
	FOnSlotsUpdated OnSlotsUpdated;

	UFUNCTION(BlueprintCallable, Category = "Object")
	int GetSlotsNbr() const;
	
	UFUNCTION(BlueprintCallable, Category = "Object")
	int GetFreeSlotsNbr() const;

	// Electricity
	UFUNCTION(BlueprintCallable, Category = "Object")
	bool HasEnoughElectricity() const { return bHasEnoughElectricity; }
	
	UFUNCTION(BlueprintCallable, Category = "Object")
	void SetHasEnoughElectricity(bool bEnoughElectricity);

	// Activated
	UFUNCTION(BlueprintCallable, Category = "Object")
	bool IsActivated() const { return bIsActivated && bHasEnoughElectricity; }

	UFUNCTION(BlueprintCallable, Category = "Object")
	void SetIsActivated(bool bActivated);

	UFUNCTION(BlueprintCallable, Category = "Object")
	void SetWillBeRemoved(bool bRemoved);

	// Using
	void StartUsing(ANPC* NPC);
	void StopUsing(ANPC* NPC);

	virtual bool StartUsingImplementation(ANPC* NPC);
	virtual bool StopUsingImplementation(ANPC* NPC);

	UFUNCTION(BlueprintCallable, Category = "Object")
	EObjectType GetObjectType() const { return ObjectType; }

	UFUNCTION(BlueprintCallable, Category = "Object")
	ENPCActionType GetNPCUseAction() const { return NPCUseAction; }

	UFUNCTION(BlueprintCallable, Category = "Object")
	ENPCActionType GetNPCMoveAction() const { return NPCMoveAction; }

	UFUNCTION(BlueprintCallable, Category = "Object")
	EResourceType GetUsingResourceTypeToCheck() const { return UsingResourceTypeToCheck; }

	UFUNCTION(BlueprintCallable, Category = "Object")
	void SetBuildData(UBuildData* NewData) { BuildData = NewData; }
	
	UFUNCTION(BlueprintCallable, Category = "Object")
	UBuildData* GetBuildData() const { return BuildData; }
	
	UFUNCTION(BlueprintCallable, Category = "Object")
	void DestroyObject();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int RoomId = UBuildSubsystem::InvalidRoomId;

	bool bIsBeingDestroyed;

	void UpdateMaterialState() const;

	UPROPERTY()
	TArray<FIntPoint> OccupiedCells;

	// Actions
	UFUNCTION()
	virtual void InitActions();

	UFUNCTION(BlueprintCallable, Category = "Object")
	UResourceComponent* GetHealthComponent() const { return HealthComponent; }
	
protected:
	UPROPERTY()
	TArray<USlotComponent*> Slots; 
	
	UPROPERTY()
	TObjectPtr<ACrowdedGameState> GameState{nullptr};
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> MeshComp{nullptr};
	
	UPROPERTY()
	bool bCanBeUsed = true;

	UPROPERTY()
	bool bHasEnoughElectricity = true;

	UPROPERTY()
	bool bIsActivated = true;

	UPROPERTY()
	bool bWillBeRemoved = false;

	UPROPERTY()
	EObjectType ObjectType = EObjectType::Default;

	UPROPERTY()
	ENPCActionType NPCUseAction = ENPCActionType::Idle;

	UPROPERTY()
	ENPCActionType NPCMoveAction = ENPCActionType::MoveToWork;
	
	UPROPERTY(EditAnywhere)
	EResourceType UsingResourceTypeToCheck = EResourceType::None;

	UPROPERTY()
	TObjectPtr<UBuildData> BuildData{nullptr};

	UPROPERTY()
	TObjectPtr<UBuildSubsystem> BuildSubsystem{nullptr};

	UPROPERTY()
	TObjectPtr<ACrowdedPlayerController> CrowdedPlayerController{nullptr};

	UPROPERTY()
	TObjectPtr<ACrowdedGameState> CrowdedGameState{nullptr};

	UPROPERTY()
	TObjectPtr<AGameHUD> GameHUD{nullptr};

	UPROPERTY()
	TObjectPtr<UBuildableRegistrySubsystem> BRS{nullptr};
	
	UPROPERTY()
	TArray<TWeakObjectPtr<ANPC>> UsingNPCs;

	// Materials
	UPROPERTY()
	TObjectPtr<UMaterialInterface> NormalMaterial{nullptr};

	UPROPERTY()
	TObjectPtr<UMaterialInterface> WillBeRemovedMaterial{nullptr};

	UPROPERTY()
	TObjectPtr<UMaterialInterface> DisabledMaterial{nullptr};

	// Actions
	UPROPERTY()
	TObjectPtr<UActionComponent> ActionComponent{nullptr};

	// Health
	UPROPERTY(EditAnywhere)
	TObjectPtr<UResourceComponent> HealthComponent{nullptr};
};
