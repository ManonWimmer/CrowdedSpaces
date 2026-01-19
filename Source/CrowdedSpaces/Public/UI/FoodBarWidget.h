#pragma once

#include "CoreMinimal.h"
#include "CustomWidget.h"
#include "FoodBarWidget.generated.h"

UCLASS()
class CROWDEDSPACES_API UFoodBarWidget : public UCustomWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	virtual void Init_Implementation() override;
	virtual void Reset_Implementation() override;

public:
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	AActor* OwningActor; // sinon on peut pas le recup quand il est mis en widget component
	
};
