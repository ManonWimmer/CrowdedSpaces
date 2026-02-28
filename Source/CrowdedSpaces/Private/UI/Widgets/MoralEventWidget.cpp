#include "UI/Widgets/MoralEventWidget.h"

void UMoralEventWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMoralEventWidget::Init_Implementation()
{
	Super::Init_Implementation();
	// En BP, setup binds avec Events Manager ? 
}

void UMoralEventWidget::Reset_Implementation()
{
	Super::Init_Implementation();
	// En BP, reset texts ? 
}

void UMoralEventWidget::Setup_Implementation(AActor* SelectableActor)
{
	Super::Setup_Implementation(SelectableActor);
}

void UMoralEventWidget::Unsetup_Implementation()
{
	Super::Unsetup_Implementation();
}

void UMoralEventWidget::Update_Implementation(const UMoralEvent* MoralEvent)
{
	// En BP, update texts 
}
