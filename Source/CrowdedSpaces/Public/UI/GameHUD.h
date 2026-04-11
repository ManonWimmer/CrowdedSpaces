#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MoralEvent/MoralEvent.h"
#include "Selection/SelectionType.h"
#include "WidgetStartupConfig.h"
#include "Grid/GridRoom.h"
#include "GameHUD.generated.h"

class UCustomWidget;

UCLASS()
class CROWDEDSPACES_API AGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	// Generic functions
	template<typename T>
	TObjectPtr<T> GetOrCreateWidget(TSubclassOf<UCustomWidget> WidgetClass);

	UFUNCTION()
	void CreateNewWidget(TSubclassOf<UCustomWidget> WidgetClass, ESlateVisibility InitialVisibility);

	UFUNCTION()
	UCustomWidget* ShowWidget(TSubclassOf<UCustomWidget> WidgetClass, bool bShow, ESlateVisibility VisibilityOnShow);

	UPROPERTY(EditAnywhere, Category="Widgets")
	TArray<FWidgetStartupConfig> StartupWidgetsConfig;

	UFUNCTION()
	void CreateStartupWidgets();
	
	// Build
	UFUNCTION(BlueprintCallable, Category="Widgets")
	void ShowBuildWidget(bool bShow);

	// Player Resources (money etc)
	UFUNCTION(BlueprintCallable, Category="Widgets")
	void ShowPlayerResourcesWidget(bool bShow);

	// Selection
	void ShowSelectionWidget(AActor* SelectableActor, bool bShow, ESelectionType SelectionType);
	
	void ShowSelectionWidget(FGridRoom& Room, bool bShow, ESelectionType SelectionType);

	UCustomWidget* GetWidgetFromSelectionType(ESelectionType Type);

	UFUNCTION(BlueprintCallable, Category="Widgets")
	void HideCurrentSelectionWidget();
	
	// Moral Event
	UFUNCTION(BlueprintCallable, Category="Widgets")
	void ShowMoralEventWidget(bool bShow);

	UFUNCTION(BlueprintCallable, Category="Widgets")
	void UpdateMoralEventWidget(const UMoralEvent* MoralEvent);

	// Time
	UFUNCTION(BlueprintCallable, Category="Widgets")
	void ShowTimeWidget(bool bShow);
	
private:
	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController;

	// Build
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UCustomWidget> BuildWidgetBP;

	// Player Resources (money etc)
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UCustomWidget> PlayerResourcesWidgetBP;

	// Selection
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UCustomWidget> RoomSelectionWidgetBP;

	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UCustomWidget> StorageRoomSelectionWidgetBP;

	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UCustomWidget> GeneratorSelectionWidgetBP;

	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UCustomWidget> NPCSelectionWidgetBP;

	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UCustomWidget> FoodSelectionWidgetBP;

	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UCustomWidget> BedSelectionWidgetBP;

	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UCustomWidget> TrainingStationSelectionWidgetBP;

	UPROPERTY(EditAnywhere, Category="Widgets")
	AActor* CurrentlySelectedActor = nullptr;

	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UCustomWidget> CurrentlyShownSelectionWidgetBP = nullptr;
	
	// Moral Event
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UCustomWidget> MoralEventWidgetBP;

	// Debug
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UCustomWidget> DebugWidgetBP;

	// Time
	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UCustomWidget> TimeBP;

	UPROPERTY()
	TMap<TSubclassOf<UCustomWidget>, TObjectPtr<UCustomWidget>> WidgetInstances;

	int CursorOverUI = 0;

	bool bUIClickThisFrame = false;

	FGridRoom& CurrentlySelectedRoom = *(new FGridRoom());
};

