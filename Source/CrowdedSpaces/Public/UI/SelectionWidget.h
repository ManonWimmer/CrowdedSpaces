#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectionWidget.generated.h"

class ISelectableStatProvider;

UCLASS()
class CROWDEDSPACES_API USelectionWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TArray<TScriptInterface<ISelectableStatProvider>> BoundStats;

	UPROPERTY()
	AActor* SelectedActor = nullptr;

	UPROPERTY()
	FString ActorDisplayName = "";

public:
	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void Init();

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void Reset();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateSelection(const FString& DisplayName, const TMap<FString, FString>& Stats);

	void BindToSelectable(AActor* SelectableActor, FString DisplayName);

	// Unbind proprement
	void Unbind();
	
	// Callback générique
	UFUNCTION()
	void OnAnyStatUpdated(FName StatId, float NewValue);
};
