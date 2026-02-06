#pragma once

#include "CoreMinimal.h"
#include "UI/CustomWidget.h"
#include "Selection/SelectionType.h"
#include "SelectionWidget.generated.h"

class ISelectableStatProvider;

UCLASS()
class CROWDEDSPACES_API USelectionWidget : public UCustomWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	virtual void Init_Implementation() override;
	virtual void Reset_Implementation() override;
	virtual void Setup_Implementation(AActor* SelectableActor) override;
	virtual void Unsetup_Implementation() override;

	UPROPERTY()
	TArray<TScriptInterface<ISelectableStatProvider>> BoundStats;

	UPROPERTY()
	TObjectPtr<AActor> SelectedActor = nullptr;

	UPROPERTY()
	FString ActorDisplayName = "";

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateSelection(const FString& DisplayName, const TMap<FString, FString>& Stats, ESelectionType SelectionType);

	UPROPERTY()
	ESelectionType CurrentSelectionType = ESelectionType::Default;
};
