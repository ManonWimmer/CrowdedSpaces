#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionComponent.generated.h"

class UWidgetComponent;
class UAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWDEDSPACES_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UActionComponent();
	
	TArray<TObjectPtr<UAction>> GetAvailableActions(AActor* Instigator);

	void SetupActions(const TArray<TObjectPtr<UAction>>& NewActions);

protected:
	UPROPERTY()
	TArray<TObjectPtr<UAction>> Actions;
};
