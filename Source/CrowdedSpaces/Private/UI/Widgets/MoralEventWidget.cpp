#include "UI/Widgets/MoralEventWidget.h"

void UMoralEventWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMoralEventWidget::Update_Implementation(const UMoralEventData* EventData)
{
	// En BP, update texts 
}

void UMoralEventWidget::Reset_Implementation()
{
	Super::Init_Implementation();
	// En BP, reset texts ? 
}

void UMoralEventWidget::Init_Implementation()
{
	Super::Init_Implementation();
	// En BP, setup binds avec Events Manager ? 
}