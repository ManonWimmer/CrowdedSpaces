#pragma once

#include "CoreMinimal.h"
#include "Build/BuildableObject.h"
#include "Selection/Selectable.h"
#include "Selection/SelectableStatProvider.h"
#include "BuildableBed.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsBedAvailableChanged, bool, Value);

UCLASS()
class CROWDEDSPACES_API ABuildableBed : public ABuildableObject, public ISelectable, public ISelectableStatProvider
{
	GENERATED_BODY()

public:
	ABuildableBed();

	bool IsAvailable() const { return bIsAvailable; }
	
	void SetAvailable(bool NewAvailable);

	// Selectable
	virtual void OnSelected() override;
	virtual void OnDeselected() override;
	
	virtual FString GetDisplayName() const override;
	virtual TObjectPtr<AActor> GetSelectableActor() override;

	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnStatChanged;

	UPROPERTY(BlueprintAssignable)
	FOnIsBedAvailableChanged OnIsBedAvailableChanged;
	
	virtual TArray<FStat> GetCurrentValues() const override;
	virtual FOnStatChanged& GetOnStatChanged() override { return OnStatChanged; }

	UFUNCTION(BlueprintCallable, Category = "Bed")
	bool GetIsAvailable() const { return bIsAvailable; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	bool bIsAvailable = true;
};
