#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "Selection/Selectable.h"
#include "Fire.generated.h"

class ACrowdedGameState;
class UActionComponent;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFireExtinguished, AFire*, Fire);

UCLASS()
class CROWDEDSPACES_API AFire : public AActor, public ISelectable
{
	GENERATED_BODY()

public:
	AFire();

	void InitActions();
	void ExtinguishFire();
	
	virtual void OnSelected() override;
	virtual void OnDeselected() override;

	FIntPoint GridCoords;

	UPROPERTY(BlueprintAssignable)
	FOnFireExtinguished OnFireExtinguished;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> NiagaraComponent{nullptr};

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereCollision{nullptr};

	UPROPERTY(EditAnywhere)
	TObjectPtr<UActionComponent> ActionComponent{nullptr};

	UPROPERTY()
	TObjectPtr<ACrowdedGameState> GameState{nullptr};
};
