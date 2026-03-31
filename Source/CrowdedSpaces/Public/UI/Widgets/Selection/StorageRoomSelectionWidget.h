#pragma once

#include "CoreMinimal.h"
#include "UI/CustomWidget.h"
#include "StorageRoomSelectionWidget.generated.h"

UCLASS()
class CROWDEDSPACES_API UStorageRoomSelectionWidget : public UCustomWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Widget")
	void SetupRoom(const FGridRoom& Room);

	void SetupRoom_Implementation(const FGridRoom& Room);
	
protected:
	virtual void NativeConstruct() override;
	
	virtual void Init_Implementation() override;
	virtual void Reset_Implementation() override;
	virtual void Setup_Implementation(AActor* SelectableActor) override;
	virtual void Unsetup_Implementation() override;

	UPROPERTY()
	TObjectPtr<AActor> SelectedActor = nullptr;
};
