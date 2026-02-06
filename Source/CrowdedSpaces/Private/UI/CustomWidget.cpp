#include "UI/CustomWidget.h"

FReply UCustomWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		//if (GEngine)
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "left click on UI consumed");
		
		return FReply::Handled(); // Bloque le click vers le monde
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UCustomWidget::Init_Implementation()
{
}

void UCustomWidget::Reset_Implementation()
{
}

void UCustomWidget::Setup_Implementation(AActor* SelectableActor)
{
}

void UCustomWidget::Unsetup_Implementation()
{
}
