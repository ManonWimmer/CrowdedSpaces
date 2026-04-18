#include "Action/ActionComponent.h"

#include "Camera/CameraComponent.h"
#include "Camera/FreeCameraPawn.h"
#include "Components/WidgetComponent.h"
#include "Game/CrowdedGameState.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/Widgets/World/ActionWidget.h"

UActionComponent::UActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UActionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UActionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

TArray<TObjectPtr<UAction>> UActionComponent::GetAvailableActions(AActor* Instigator)
{
	return Actions;
}

void UActionComponent::SetupActions(const TArray<TObjectPtr<UAction>>& NewActions)
{
	Actions = NewActions;
}

