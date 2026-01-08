#include "UI/GameHUD.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AGameHUD::BeginPlay()
{
	Super::BeginPlay();

	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("GameHUD begin"));	
	
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	CreateAndInitBuildWidget();
}

void AGameHUD::CreateAndInitBuildWidget()
{
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("CreateAndInitBuildWidget"));
	
	if (!BuildWidgetBP || !PlayerController) return;

	BuildWidget= CreateWidget(PlayerController, BuildWidgetBP);
	BuildWidget->AddToViewport(0);

	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Created build widget"));	
}
