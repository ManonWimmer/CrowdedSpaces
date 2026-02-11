#pragma once

#include "CoreMinimal.h"
#include "Build/ObjectType.h"
#include "AI/NPC.h"
#include "GameFramework/Actor.h"
#include "BuildableObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCUsingChanged, bool, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCComingChanged, bool, Value);

UCLASS()
class CROWDEDSPACES_API ABuildableObject : public AActor
{
	GENERATED_BODY()

public:
	ABuildableObject();

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
	bool CanBeUsed() const { return bCanBeUsed; }
	
	UFUNCTION(BlueprintCallable, Category = "Object")
	bool HasNPCComing() const { return bHasNPCComing; }

	UFUNCTION(BlueprintCallable, Category = "Object")
	bool HasNPCUsing() const { return bHasNPCUsing; }
	
	bool TryReserve(ANPC* NPC);
	bool IsReservedByOther(TObjectPtr<ANPC> NPC) const;
	void Release(ANPC* NPC);
	void StartUsing(ANPC* NPC);
	void StopUsing(ANPC* NPC);
	
	virtual void StartUsingImplementation();
	virtual void StopUsingImplementation();

	UFUNCTION(BlueprintCallable, Category = "Object")
	EObjectType GetObjectType() const { return ObjectType; }
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY()
	TWeakObjectPtr<ANPC> ComingNPC;

	UPROPERTY()
	TWeakObjectPtr<ANPC> UsingNPC;

	UPROPERTY()
	bool bCanBeUsed = true;
	
	UPROPERTY()
	bool bHasNPCUsing = false;

	UPROPERTY()
	bool bHasNPCComing = false;

	UPROPERTY()
	EObjectType ObjectType = EObjectType::Default;
};
