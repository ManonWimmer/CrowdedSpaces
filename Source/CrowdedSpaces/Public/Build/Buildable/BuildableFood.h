#pragma once

#include "CoreMinimal.h"
#include "Build/BuildableObject.h"
#include "Selection/Selectable.h"
#include "Selection/SelectableStatProvider.h"
#include "BuildableFood.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIsFoodAvailableChanged, bool, Value);

UCLASS()
class CROWDEDSPACES_API ABuildableFood : public ABuildableObject, public ISelectable, public ISelectableStatProvider
{
	GENERATED_BODY()

public:
	ABuildableFood();

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
	FOnIsFoodAvailableChanged OnIsFoodAvailableChanged;
	
	virtual TArray<FStat> GetCurrentValues() const override;
	virtual FOnStatChanged& GetOnStatChanged() override { return OnStatChanged; }

	UFUNCTION(BlueprintCallable, Category = "Food")
	bool GetIsAvailable() const { return bIsAvailable; }

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	bool bIsAvailable = true;
};
