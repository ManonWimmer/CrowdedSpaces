#pragma once

#include "CoreMinimal.h"
#include "BuildableObject.h"
#include "Production/ProductionComponent.h"
#include "Selection/Selectable.h"
#include "BuildableGenerator.generated.h"

UCLASS()
class CROWDEDSPACES_API ABuildableGenerator : public ABuildableObject, public ISelectable
{
	GENERATED_BODY()

public:
	ABuildableGenerator();

	// Selectable
	virtual void OnSelected() override;
	virtual void OnDeselected() override;
	
	virtual FString GetDisplayName() const override;
	virtual AActor* GetSelectableActor() override;

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere)
	UProductionComponent* ProductionComponent;
};
