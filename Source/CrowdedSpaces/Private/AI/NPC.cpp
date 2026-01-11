#include "AI/NPC.h"

#include "UI/FoodBarWidget.h"

ANPC::ANPC()
{
	FoodComponent = CreateDefaultSubobject<UFoodComponent>(TEXT("FoodComponent"));
	FoodBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("FoodBarWidget"));
	FoodBarWidget->SetupAttachment(GetMesh());
}

void ANPC::BeginPlay()
{
	Super::BeginPlay();

	// Cast food bar widget class to food bar widget -> set owning actor
	if (FoodBarWidget)
	{
		if (UUserWidget* UserWidget = FoodBarWidget->GetUserWidgetObject())
		{
			if (UFoodBarWidget* FoodWidget = Cast<UFoodBarWidget>(UserWidget))
			{
				FoodWidget->OwningActor = this;
				FoodWidget->Init();
			}
		}
	}
	
	StartRemoveFood();
}

void ANPC::RemoveFood() const
{
	FoodComponent->RemoveFood(RemoveFoodPerInterval);
}

void ANPC::StartRemoveFood()
{
	GetWorldTimerManager().SetTimer(
		RemoveFoodTimerHandle,
		this,
		&ANPC::RemoveFood,
		RemoveFoodInterval,
		true // looping
	);
}


