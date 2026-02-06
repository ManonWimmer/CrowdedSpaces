#include "UI/Widgets/PlayerResourcesWidget.h"

void UPlayerResourcesWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerResourcesWidget::Init_Implementation()
{
	Super::Init_Implementation();
	// En BP, setup money, electricity, oxygen & food
}

void UPlayerResourcesWidget::Reset_Implementation()
{
	Super::Reset_Implementation();
}

void UPlayerResourcesWidget::Setup_Implementation(AActor* SelectableActor)
{
	Super::Setup_Implementation(SelectableActor);
}

void UPlayerResourcesWidget::Unsetup_Implementation()
{
	Super::Unsetup_Implementation();
}
