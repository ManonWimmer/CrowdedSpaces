#pragma once

#include "CoreMinimal.h"
#include "Build/BuildableObject.h"
#include "Selection/Selectable.h"
#include "Selection/SelectableStatProvider.h"
#include "BuildableBed.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsBedAvailableChanged, bool, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNPCComingToBedChanged, bool, Value);

UCLASS()
class CROWDEDSPACES_API ABuildableBed : public ABuildableObject, public ISelectable, public ISelectableStatProvider
{
	GENERATED_BODY()

public:
	ABuildableBed();

	UFUNCTION(BlueprintCallable, Category = "Bed")
	bool IsAvailable() const { return bIsAvailable; }
	
	UFUNCTION(BlueprintCallable, Category = "Bed")
	bool HasNPCComing() const { return bHasNPCComing; }
	
	void SetAvailable(bool NewAvailable);
	void SetHasNPCComing(bool NewAvailable);

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
	FOnIsBedAvailableChanged OnIsBedAvailableChanged;
	
	virtual TArray<FStat> GetCurrentValues() const override;
	virtual FOnStatChanged& GetOnStatChanged() override { return OnStatChanged; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	bool bIsAvailable = true;
	bool bHasNPCComing = false;
};
