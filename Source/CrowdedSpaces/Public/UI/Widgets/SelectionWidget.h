#pragma once

#include "CoreMinimal.h"
#include "UI/CustomWidget.h"
#include "SelectionWidget.generated.h"

UCLASS()
class CROWDEDSPACES_API USelectionWidget : public UCustomWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	virtual void Init_Implementation() override;
	virtual void Reset_Implementation() override;
	virtual void Setup_Implementation(AActor* SelectableActor) override;
	virtual void Unsetup_Implementation() override;

	UPROPERTY()
	TObjectPtr<AActor> SelectedActor = nullptr;
};
