#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MoralEvent/MoralEvent.h"
#include "WidgetStartupConfig.h"
#include "GameHUD.generated.h"

enum class ESelectionType : uint8;
class UCustomWidget;
class USelectionWidget;
class UGeneratorSelectionWidget;

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
	UFUNCTION(BlueprintCallable, Category="Widgets")
	void ShowSelectionWidget(AActor* SelectableActor, bool bShow, ESelectionType SelectionType);
	
	UFUNCTION()
	USelectionWidget* GetSelectionWidget() { return GetOrCreateWidget<USelectionWidget>(SelectionWidgetBP); }
	
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
	TSubclassOf<UCustomWidget> SelectionWidgetBP;

	UPROPERTY(EditAnywhere, Category="Widgets")
	TSubclassOf<UCustomWidget> RoomSelectionWidgetBP;

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
};

