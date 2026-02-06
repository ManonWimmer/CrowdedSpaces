#include "UI/Widgets/BuildWidget.h"

void UBuildWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UBuildWidget::Reset_Implementation()
{
	Super::Reset_Implementation();
	// En BP, reset selected objects
}

void UBuildWidget::Setup_Implementation(AActor* SelectableActor)
{
	Super::Setup_Implementation(SelectableActor);
}

void UBuildWidget::Unsetup_Implementation()
{
	Super::Unsetup_Implementation();
}

void UBuildWidget::Init_Implementation()
{
	Super::Init_Implementation();
	// En BP, setup grid avec x btns setup pour les x build data du build manager
}

