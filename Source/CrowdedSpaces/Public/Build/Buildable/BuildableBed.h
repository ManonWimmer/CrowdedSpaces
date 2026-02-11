#pragma once

#include "CoreMinimal.h"
#include "AI/NPC.h"
#include "Build/BuildableObject.h"
#include "Selection/Selectable.h"
#include "Selection/SelectableStatProvider.h"
#include "BuildableBed.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCSleepingChanged, bool, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCComingToBedChanged, bool, Value);

UCLASS()
class CROWDEDSPACES_API ABuildableBed : public ABuildableObject, public ISelectable, public ISelectableStatProvider
{
	GENERATED_BODY()

public:
	ABuildableBed();

	UFUNCTION(BlueprintCallable, Category = "Bed")
	bool HasNPCSleeping() const { return bHasNPCSleeping; }
	
	UFUNCTION(BlueprintCallable, Category = "Bed")
	bool HasNPCComing() const { return bHasNPCComing; }
	
	// Selectable
	virtual void OnSelected() override;
	virtual void OnDeselected() override;
	
	virtual FString GetDisplayName() const override;
	virtual TObjectPtr<AActor> GetSelectableActor() override;

	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnStatChanged;

	UPROPERTY(BlueprintAssignable)
	FOnNPCComingToBedChanged OnNPCComingToBedChanged;

	UPROPERTY(BlueprintAssignable)
	FOnNPCSleepingChanged OnNPCSleepingChanged;
	
	virtual TArray<FStat> GetCurrentValues() const override;
	virtual FOnStatChanged& GetOnStatChanged() override { return OnStatChanged; }

	bool TryReserve(ANPC* NPC);
	bool IsReservedByOther(TObjectPtr<ANPC> NPC);
	void Release(ANPC* NPC);
	void StartSleeping(ANPC* NPC);
	void StopSleeping(ANPC* NPC);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	bool bIsAvailable = true;
	bool bHasNPCComing = false;

	UPROPERTY()
	bool bHasNPCSleeping = false;
	
	UPROPERTY()
	TWeakObjectPtr<ANPC> ComingNPC;

	UPROPERTY()
	TWeakObjectPtr<ANPC> EatingNPC;
};
