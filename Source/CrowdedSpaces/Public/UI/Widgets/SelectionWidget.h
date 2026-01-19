#pragma once

#include "CoreMinimal.h"
#include "UI/CustomWidget.h"
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

	UPROPERTY()
	TArray<TScriptInterface<ISelectableStatProvider>> BoundStats;

	UPROPERTY()
	AActor* SelectedActor = nullptr;

	UPROPERTY()
	FString ActorDisplayName = "";

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateSelection(const FString& DisplayName, const TMap<FString, FString>& Stats);

	void BindToSelectable(AActor* SelectableActor, FString DisplayName);

	// Unbind proprement
	void Unbind();
	
	// Callback générique
	UFUNCTION()
	void OnAnyStatUpdated(FName StatId, float NewValue);
};
