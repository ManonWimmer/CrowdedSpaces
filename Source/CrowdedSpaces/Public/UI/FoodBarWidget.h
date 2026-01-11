#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FoodBarWidget.generated.h"

UCLASS()
class CROWDEDSPACES_API UFoodBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void Init();

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	AActor* OwningActor; // sinon on peut pas le recup quand il est mis en widget component
};
