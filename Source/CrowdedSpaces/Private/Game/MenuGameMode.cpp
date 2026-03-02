#include "MenuGameMode.h"

#include "Blueprint/UserWidget.h"

void AMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!MenuWidgetClass)
		return;
	
	UUserWidget* Widget = CreateWidget(GetWorld(), MenuWidgetClass);
	if (!Widget)
		return;
	
	Widget->AddToViewport();

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController)
		return;
	
	PlayerController->bShowMouseCursor = true;
	PlayerController->SetInputMode(FInputModeUIOnly());
}
