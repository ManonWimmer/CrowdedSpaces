#include "UI/SelectionWidget.h"

#include "Selection/SelectableStatProvider.h"

void USelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USelectionWidget::BindToSelectable(AActor* SelectableActor, FString DisplayName)
{
	if (!SelectableActor) return;
	
	Unbind();

	SelectedActor = SelectableActor;
	ActorDisplayName = DisplayName;
	BoundStats.Empty();
	
	TArray<UActorComponent*> Components;
	SelectableActor->GetComponents(Components);

	TMap<FString, FString> StatsToDisplay;

	for (UActorComponent* Comp : Components)
	{
		if (Comp->Implements<USelectableStatProvider>())
		{
			TScriptInterface<ISelectableStatProvider> StatProvider(Comp);
			BoundStats.Add(StatProvider);
			
			StatsToDisplay.Add(
				StatProvider->GetStatDisplayName(),
				FString::SanitizeFloat(StatProvider->GetCurrentValue())
			);
			
			StatProvider->GetOnStatChanged().AddDynamic(this, &USelectionWidget::OnAnyStatUpdated);
		}
	}
	
	UpdateSelection(DisplayName, StatsToDisplay);
}

void USelectionWidget::Unbind()
{
	for (auto& Stat : BoundStats)
	{
		Stat->GetOnStatChanged().RemoveAll(this);
	}
	
	BoundStats.Empty();
	SelectedActor = nullptr;
	ActorDisplayName = "";
}

void USelectionWidget::OnAnyStatUpdated(FName StatId, float NewValue)
{
	if (!SelectedActor) return;

	TMap<FString, FString> StatsToDisplay;

	// Recrée toutes les stats
	for (auto& Stat : BoundStats)
	{
		StatsToDisplay.Add(
			Stat->GetStatDisplayName(),
			FString::SanitizeFloat(Stat->GetCurrentValue())
		);
	}
	
	UpdateSelection(ActorDisplayName, StatsToDisplay);
}

void USelectionWidget::Init_Implementation()
{
}

void USelectionWidget::Reset_Implementation()
{
}

