#include "UI/Widgets/SelectionWidget.h"

#include "Selection/SelectableStatProvider.h"

void USelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USelectionWidget::BindToSelectable(AActor* SelectableActor, FString DisplayName, ESelectionType SelectionType)
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

	CurrentSelectionType = SelectionType;
	UpdateSelection(DisplayName, StatsToDisplay, CurrentSelectionType);
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

void USelectionWidget::OnAnyStatUpdated(FName StatId, FString NewValue)
{
	if (!SelectedActor)
		return;

	TMap<FString, FString> StatsToDisplay;

	// Recréer toutes les stats
	// todo: pas recréer mais check si statid == , change value seulement pour 1
	for (const TScriptInterface<ISelectableStatProvider>& Stat : BoundStats)
	{
		for (const auto StatValue : Stat->GetCurrentValues())
		{
			StatsToDisplay.Add(StatValue.Key, StatValue.Value);
		}
	}
	
	UpdateSelection(ActorDisplayName, StatsToDisplay, CurrentSelectionType);
}

void USelectionWidget::Init_Implementation()
{
	Super::Init_Implementation();
}

void USelectionWidget::Reset_Implementation()
{
	Super::Reset_Implementation();
}