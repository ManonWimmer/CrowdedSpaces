#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MenuGameMode.generated.h"

class UUserWidget;

UCLASS()
class CROWDEDSPACES_API AMenuGameMode : public AGameMode
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> MenuWidgetClass;
};
