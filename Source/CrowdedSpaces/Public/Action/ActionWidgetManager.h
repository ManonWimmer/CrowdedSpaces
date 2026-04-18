#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ActionWidgetManager.generated.h"

class UActionWidget;
class UAction;
class APlayerController;

UCLASS()
class CROWDEDSPACES_API UActionWidgetManager : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize(APlayerController* InPC);

	void ShowForActor(AActor* Actor);
	void Hide();

	void Tick(float DeltaTime);

private:
	void UpdateScreenPosition() const;

private:
	UPROPERTY()
	TObjectPtr<APlayerController> PC;

	UPROPERTY()
	TObjectPtr<UActionWidget> Widget;

	UPROPERTY()
	TObjectPtr<AActor> CurrentActor;

	UPROPERTY()
	TSubclassOf<UActionWidget> WidgetClass;

	bool bIsVisible = false;
};
