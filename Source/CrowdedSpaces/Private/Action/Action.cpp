#include "Action/Action.h"

#include "Action/ActionSubsystem.h"

void UAction::Initialize(const UWorld* World)
{
	if (!World) return;

	ActionSubsystem = World->GetSubsystem<UActionSubsystem>();
}

bool UAction::CanExecute_Implementation(AActor* Instigator) const
{
	return true;
}

void UAction::Execute_Implementation(AActor* Instigator)
{
	ActionSubsystem->HideActions();
}

void UAction::ExecuteWrapper(AActor* Instigator)
{
	Execute(Instigator);
}

bool UAction::CanExecuteWrapper(AActor* Instigator) const
{
	return CanExecute(Instigator);
}
