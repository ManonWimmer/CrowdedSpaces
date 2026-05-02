#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Object/UsableObject.h"
#include "GameFramework/Actor.h"
#include "Selection/Selectable.h"
#include "Fire.generated.h"

class ACrowdedGameState;
class UActionComponent;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFireExtinguished, AFire*, Fire);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireExtinguishProgress);

UCLASS()
class CROWDEDSPACES_API AFire : public AUsableObject, public ISelectable
{
	GENERATED_BODY()

public:
	AFire();
	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	virtual void InitActions() override;
	void ExtinguishFire();
	
	virtual void OnSelected() override;
	virtual void OnDeselected() override;

	virtual bool StartUsingImplementation(ANPC* NPC) override;
	virtual bool StopUsingImplementation(ANPC* NPC) override;

	UFUNCTION(BlueprintCallable)
	float GetFireExtinguishProgress() const;

	FIntPoint GridCoords;

	UPROPERTY(BlueprintAssignable)
	FOnFireExtinguished OnFireExtinguished;

	UPROPERTY(BlueprintAssignable)
	FOnFireExtinguishProgress OnFireExtinguishProgress;

protected:
	void ApplyDamage();
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent{nullptr};

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereCollision{nullptr};

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> ClickCollision{nullptr};

	UPROPERTY()
	TArray<TObjectPtr<AActor>> OverlappingActors;

	UPROPERTY()
	FTimerHandle DamageTimerHandle;

	UPROPERTY(EditAnywhere)
	float DamageInterval = 1.0f;

	UPROPERTY(EditAnywhere)
	float DamageAmount = 10.0f;

	// Extinguish
	UPROPERTY()
	TArray<TWeakObjectPtr<ANPC>> NPCsExtinguishing;

	float CurrentExtinguishProgress = 0.f;

	UPROPERTY(EditAnywhere)
	float TimeToExtinguish = 3.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> ExtinguishProgressWidget{nullptr};

	UPROPERTY()
	TObjectPtr<APlayerController> PlayerController{nullptr};
};
