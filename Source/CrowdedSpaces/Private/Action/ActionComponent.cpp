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

	if (!GetOwner()) return;

	// Create widget component world & get class from game state
	WidgetComponent = NewObject<UWidgetComponent>(GetOwner());
	if (!WidgetComponent) return;

	WidgetComponent->RegisterComponent();
	WidgetComponent->AttachToComponent(
		GetOwner()->GetRootComponent(),
		FAttachmentTransformRules::KeepRelativeTransform
	);

	WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	WidgetComponent->SetVisibility(false);
	
	const UWorld* World = GetWorld();
	if (!World) return;

	const ACrowdedGameState* GameState = World->GetGameState<ACrowdedGameState>();
	if (!GameState) return;
	
	WidgetComponent->SetWidgetClass(GameState->ActionWidgetClass);

	ShowWidget();
}

void UActionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Widgets world look at camera
	// todo : opti sans get & cast dans begin play (pareil tick npc pour ses widgets)
	if (!WidgetComponent) return;

	const APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	const AFreeCameraPawn* CamPawn = Cast<AFreeCameraPawn>(PC->GetPawn());
	if (!CamPawn) return;

	const UCameraComponent* Cam = CamPawn->GetCameraComponent();
	if (!Cam) return;

	const FVector CameraLocation = Cam->GetComponentLocation();
	const FVector WidgetLocation = WidgetComponent->GetComponentLocation();

	const FRotator LookAtWidgetRotation = UKismetMathLibrary::FindLookAtRotation(WidgetLocation, CameraLocation);

	const FRotator YawOnlyWidgetRotation(0.f, LookAtWidgetRotation.Yaw, 0.f);

	WidgetComponent->SetWorldRotation(YawOnlyWidgetRotation);
}

TArray<TObjectPtr<UAction>> UActionComponent::GetAvailableActions(AActor* Instigator)
{
	return Actions;
}

void UActionComponent::SetupActions(const TArray<TObjectPtr<UAction>>& NewActions)
{
	Actions = NewActions;
}

void UActionComponent::ShowWidget() const
{
	if (WidgetComponent)
	{
		WidgetComponent->SetVisibility(true);

		UActionWidget* Widget = Cast<UActionWidget>(WidgetComponent->GetUserWidgetObject());
		if (!Widget) return;

		Widget->SetupActions(Actions);
	}
}

void UActionComponent::HideWidget() const
{
	if (WidgetComponent)
	{
		WidgetComponent->SetVisibility(false);
	}
}

