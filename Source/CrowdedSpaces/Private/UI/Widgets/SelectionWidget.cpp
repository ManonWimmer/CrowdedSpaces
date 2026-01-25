#include "UI/Widgets/SelectionWidget.h"

#include "Selection/SelectableStatProvider.h"

void USelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USelectionWidget::BindToSelectable(AActor* SelectableActor, FString DisplayName)
{
	if (!SelectableActor)
		return;
	
	Unbind();

	SelectedActor = SelectableActor;
	ActorDisplayName = DisplayName;
	BoundStats.Empty();
	
	TMap<FString, FString> StatsToDisplay;
	
	// Check interface in actor
	if (ISelectableStatProvider* ActorProvider =
		Cast<ISelectableStatProvider>(SelectableActor))
	{
		BoundStats.Add(SelectableActor);

		for (const FStat& StatValue : ActorProvider->GetCurrentValues())
		{
			StatsToDisplay.Add(StatValue.Key, StatValue.Value);
		}

		ActorProvider->GetOnStatChanged().AddDynamic(
			this,
			&USelectionWidget::OnAnyStatUpdated
		);
	}

	// Check interface in components
	TArray<UActorComponent*> Components;
	SelectableActor->GetComponents(Components);
	
	for (UActorComponent* Comp : Components)
	{
		if (Comp->Implements<USelectableStatProvider>())
		{
			TScriptInterface<ISelectableStatProvider> StatProvider(Comp);
			BoundStats.Add(StatProvider);
			
			for (const FStat& StatValue : StatProvider->GetCurrentValues())
			{
				StatsToDisplay.Add(StatValue.Key, StatValue.Value);
			}
			
			StatProvider->GetOnStatChanged().AddDynamic(this, &USelectionWidget::OnAnyStatUpdated);
		}
	}
	
	UpdateSelection(DisplayName, StatsToDisplay);
}

void USelectionWidget::Unbind()
{
	for (const TScriptInterface<ISelectableStatProvider>& Stat : BoundStats)
	{
		Stat->GetOnStatChanged().RemoveAll(this);
	}
	
	BoundStats.Empty();
	SelectedActor = nullptr;
	ActorDisplayName = "";
}

void USelectionWidget::OnAnyStatUpdated(FName StatId, float NewValue)
{
	if (!SelectedActor)
		return;

	TMap<FString, FString> StatsToDisplay;

	// Recrée toutes les stats
	for (const TScriptInterface<ISelectableStatProvider>& Stat : BoundStats)
	{
		for (const auto StatValue : Stat->GetCurrentValues())
		{
			StatsToDisplay.Add(StatValue.Key, StatValue.Value);
		}
	}
	
	UpdateSelection(ActorDisplayName, StatsToDisplay);
}

void USelectionWidget::Init_Implementation()
{
	Super::Init_Implementation();
}

void USelectionWidget::Reset_Implementation()
{
	Super::Reset_Implementation();
}