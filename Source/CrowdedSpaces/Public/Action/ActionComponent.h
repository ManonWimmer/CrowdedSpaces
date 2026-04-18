// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionComponent.generated.h"

class UWidgetComponent;
class UAction;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWDEDSPACES_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UActionComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ShowWidget() const;
	
	UFUNCTION(BlueprintCallable)
	void HideWidget() const;

public:
	TArray<TObjectPtr<UAction>> GetAvailableActions(AActor* Instigator);

	void SetupActions(const TArray<TObjectPtr<UAction>>& NewActions);

protected:
	UPROPERTY()
	TArray<TObjectPtr<UAction>> Actions;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> WidgetComponent{nullptr};
};
