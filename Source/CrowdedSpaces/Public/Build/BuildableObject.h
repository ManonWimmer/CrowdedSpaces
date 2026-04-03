#pragma once

#include "CoreMinimal.h"
#include "BuildSubsystem.h"
#include "Build/ObjectType.h"
#include "AI/NPC.h"
#include "AI/BTTasks/BTTask_UseBuildableObject.h"
#include "GameFramework/Actor.h"
#include "Grid/GridRoomType.h"
#include "BuildableObject.generated.h"

class UBuildableRegistrySubsystem;
class AGameHUD;
class ACrowdedGameState;
class ACrowdedPlayerController;
class UBuildData;
class UBuildSubsystem;
class USlotComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlotsUpdated);

UCLASS()
class CROWDEDSPACES_API ABuildableObject : public AActor
{
	GENERATED_BODY()

public:
	ABuildableObject();

	virtual void BeginPlay() override;

	UFUNCTION()
	void SetMesh(UStaticMesh* Mesh) const;

	UFUNCTION()
	FVector GetExtent() const;

	UFUNCTION()
	UStaticMeshComponent* GetMeshComponent() const { return MeshComp; }

	USlotComponent* GetNearestFreeSlot(const FVector& Vector);
	bool IsAvailableForReservation(const ANPC* NPC) const;

	UPROPERTY(BlueprintAssignable)
	FOnSlotsUpdated OnSlotsUpdated;
	
	UFUNCTION(BlueprintCallable, Category = "Object")
	bool CanBeUsed() const;

	UFUNCTION(BlueprintCallable, Category = "Object")
	void CheckCantBeUsedStopTask() const;
	
	UFUNCTION(BlueprintCallable, Category = "Object")
	int HasNPCComing() const;

	UFUNCTION(BlueprintCallable, Category = "Object")
	int HasNPCUsing() const;

	UFUNCTION(BlueprintCallable, Category = "Object")
	int GetSlotsNbr() const;
	
	UFUNCTION(BlueprintCallable, Category = "Object")
	int GetFreeSlotsNbr() const;

	UFUNCTION(BlueprintCallable, Category = "Object")
	bool HasEnoughElectricity() const { return bHasEnoughElectricity; }

	UFUNCTION(BlueprintCallable, Category = "Object")
	bool IsActivated() const { return bIsActivated && bHasEnoughElectricity; }

	UFUNCTION(BlueprintCallable, Category = "Object")
	void SetHasEnoughElectricity(bool bEnoughElectricity);

	UFUNCTION(BlueprintCallable, Category = "Object")
	void SetIsActivated(bool bActivated);
	
	bool TryReserve(const ANPC* NPC);
	void Release(ANPC* NPC);
	void StartUsing(ANPC* NPC);
	void StopUsing(ANPC* NPC);
	USlotComponent* ReserveSpecificSlot(ANPC* NPC, USlotComponent* Slot);

	virtual bool StartUsingImplementation(ANPC* NPC);
	virtual bool StopUsingImplementation(ANPC* NPC);

	UFUNCTION(BlueprintCallable, Category = "Object")
	EObjectType GetObjectType() const { return ObjectType; }

	UFUNCTION(BlueprintCallable, Category = "Object")
	ENPCActionWidget GetNPCAction() const { return NPCAction;}

	UFUNCTION(BlueprintCallable, Category = "Object")
	void SetBuildData(UBuildData* NewData);
	
	UFUNCTION(BlueprintCallable, Category = "Object")
	UBuildData* GetBuildData() const { return BuildData; }
	
	UFUNCTION(BlueprintCallable, Category = "Object")
	void DestroyObject();
	
	USlotComponent* GetFreeSlot();
	USlotComponent* ReserveSlot(ANPC* NPC);
	void ReleaseSlot(ANPC* NPC);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int RoomId = UBuildSubsystem::InvalidRoomId; 
	
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
	EObjectType ObjectType = EObjectType::Default;

	UPROPERTY()
	ENPCActionWidget NPCAction = ENPCActionWidget::Idle;

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
};
