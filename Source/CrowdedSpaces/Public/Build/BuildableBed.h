#pragma once

#include "CoreMinimal.h"
#include "BuildableObject.h"
#include "Selection/Selectable.h"
#include "Selection/SelectableStatProvider.h"
#include "BuildableBed.generated.h"

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
	virtual AActor* GetSelectableActor() override;

	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnStatChanged;
	
	virtual TArray<FStat> GetCurrentValues() const override;
	virtual FOnStatChanged& GetOnStatChanged() override { return OnStatChanged; }

protected:
	virtual void BeginPlay() override;
	
	bool bIsAvailable = true;
};
