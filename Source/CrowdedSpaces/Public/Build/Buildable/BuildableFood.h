#pragma once

#include "CoreMinimal.h"
#include "AI/NPC.h"
#include "Build/BuildableObject.h"
#include "Selection/Selectable.h"
#include "BuildableFood.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCEatingChanged, bool, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCComingToFoodChanged, bool, Value);

UCLASS()
class CROWDEDSPACES_API ABuildableFood : public ABuildableObject, public ISelectable
{
	GENERATED_BODY()

public:
	ABuildableFood();
	
	UFUNCTION(BlueprintCallable, Category = "Food")
	bool HasNPCEating() const { return bHasNPCEating; }
	
	UFUNCTION(BlueprintCallable, Category = "Food")
	bool HasNPCComing() const { return bHasNPCComing; }
	
	// Selectable
	virtual void OnSelected() override;
	virtual void OnDeselected() override;
	
	UPROPERTY(BlueprintAssignable)
	FOnNPCComingToFoodChanged OnNPCComingToFoodChanged;

	UPROPERTY(BlueprintAssignable)
	FOnNPCEatingChanged OnNPCEatingChanged;

	bool TryReserve(ANPC* NPC);
	bool IsReservedByOther(TObjectPtr<ANPC> NPC);
	void Release(ANPC* NPC);
	void StartEating(ANPC* NPC);
	void StopEating(ANPC* NPC);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	bool bHasNPCComing = false;

	UPROPERTY()
	bool bHasNPCEating = false;
	
	UPROPERTY()
	TWeakObjectPtr<ANPC> ComingNPC;

	UPROPERTY()
	TWeakObjectPtr<ANPC> EatingNPC;
};
