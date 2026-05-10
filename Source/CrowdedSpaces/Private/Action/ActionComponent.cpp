#include "Action/ActionComponent.h"

UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

TArray<TObjectPtr<UAction>> UActionComponent::GetAvailableActions(AActor* Instigator)
{
	return Actions;
}

void UActionComponent::SetupActions(const TArray<TObjectPtr<UAction>>& NewActions)
{
	Actions = NewActions;
}

