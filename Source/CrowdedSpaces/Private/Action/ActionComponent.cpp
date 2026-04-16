#include "Action/ActionComponent.h"

#include "Components/WidgetComponent.h"

UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget component");
	// set world & set class 
}

TArray<TObjectPtr<UAction>> UActionComponent::GetAvailableActions(AActor* Instigator)
{
	return Actions;
}

void UActionComponent::SetupActions(const TArray<TObjectPtr<UAction>>& NewActions)
{
	Actions = NewActions;
}


