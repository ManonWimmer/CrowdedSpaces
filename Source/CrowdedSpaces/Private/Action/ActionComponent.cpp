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

	USceneComponent* Root = GetOwner()->GetRootComponent();

	if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(Root))
	{
		float Height = Prim->Bounds.BoxExtent.Z;

		WidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, Height + 20.f));
	}

	WidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	
	WidgetComponent->SetDrawSize(FVector2D(1000, 1000));
	WidgetComponent->SetWorldScale3D(FVector(1.f)); 
	WidgetComponent->SetPivot(FVector2D(0.5f, 0.5f));
	
	WidgetComponent->SetVisibility(false);
	
	const UWorld* World = GetWorld();
	if (!World) return;

	const ACrowdedGameState* GameState = World->GetGameState<ACrowdedGameState>();
	if (!GameState) return;

	WidgetComponent->SetMaterial(0, GameState->NoDepthMaterial);
	WidgetComponent->SetWidgetClass(GameState->ActionWidgetClass);
	
	WidgetComponent->SetWindowFocusable(true);
	WidgetComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WidgetComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	WidgetComponent->SetGenerateOverlapEvents(false);
	WidgetComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
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

