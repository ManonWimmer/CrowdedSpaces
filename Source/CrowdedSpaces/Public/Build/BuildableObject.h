#pragma once

#include "CoreMinimal.h"
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCUsingChanged, bool, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCComingChanged, bool, Value);

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

	UPROPERTY(BlueprintAssignable)
	FOnNPCUsingChanged OnNPCUsingChanged;

	UPROPERTY(BlueprintAssignable)
	FOnNPCComingChanged OnNPCComingChanged;

	UFUNCTION(BlueprintCallable, Category = "Object")
	bool CanBeUsed() const;

	UFUNCTION(BlueprintCallable, Category = "Object")
	void CheckCantBeUsedStopTask() const;
	
	UFUNCTION(BlueprintCallable, Category = "Object")
	bool HasNPCComing() const { return bHasNPCComing; }

	UFUNCTION(BlueprintCallable, Category = "Object")
	bool HasNPCUsing() const { return bHasNPCUsing; }

	UFUNCTION(BlueprintCallable, Category = "Object")
	bool HasEnoughElectricity() const { return bHasEnoughElectricity; }

	UFUNCTION(BlueprintCallable, Category = "Object")
	bool IsActivated() const { return bIsActivated && bHasEnoughElectricity; }

	UFUNCTION(BlueprintCallable, Category = "Object")
	void SetHasEnoughElectricity(bool bEnoughElectricity);

	UFUNCTION(BlueprintCallable, Category = "Object")
	void SetIsActivated(bool bActivated);
	
	bool TryReserve(ANPC* NPC);
	bool IsReservedByOther(TObjectPtr<ANPC> NPC) const;
	void Release(ANPC* NPC);
	void StartUsing(ANPC* NPC);
	void StopUsing(ANPC* NPC);
	
	virtual bool StartUsingImplementation(UBTTask_UseBuildableObject* UseObjectTask);
	virtual bool StopUsingImplementation(UBTTask_UseBuildableObject* UseObjectTask);

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int GridRowsX = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int GridColumnsY = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int RoomId = -1; 
	
protected:
	UPROPERTY()
	TObjectPtr<ACrowdedGameState> GameState = nullptr;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY()
	TWeakObjectPtr<ANPC> ComingNPC = nullptr;

	UPROPERTY()
	TWeakObjectPtr<ANPC> UsingNPC = nullptr;

	UPROPERTY()
	bool bCanBeUsed = true;
	
	UPROPERTY()
	bool bHasNPCUsing = false;

	UPROPERTY()
	bool bHasNPCComing = false;

	UPROPERTY()
	bool bHasEnoughElectricity = true;

	UPROPERTY()
	bool bIsActivated = true;

	UPROPERTY()
	EObjectType ObjectType = EObjectType::Default;

	UPROPERTY()
	ENPCActionWidget NPCAction = ENPCActionWidget::Idle;

	UPROPERTY()
	TObjectPtr<UBuildData> BuildData;

	UPROPERTY()
	TObjectPtr<UBuildSubsystem> BuildSubsystem;

	UPROPERTY()
	TObjectPtr<ACrowdedPlayerController> CrowdedPlayerController;

	UPROPERTY()
	TObjectPtr<ACrowdedGameState> CrowdedGameState;

	UPROPERTY()
	TObjectPtr<AGameHUD> GameHUD;

	UPROPERTY()
	TObjectPtr<UBuildableRegistrySubsystem> BRS;
	
	UPROPERTY()
	UBTTask_UseBuildableObject* CurrentTask;
};
