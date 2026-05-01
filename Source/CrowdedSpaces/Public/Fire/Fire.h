#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "Build/BuildableObject.h"
#include "GameFramework/Actor.h"
#include "Selection/Selectable.h"
#include "Fire.generated.h"

class ACrowdedGameState;
class UActionComponent;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFireExtinguished, AFire*, Fire);

UCLASS()
class CROWDEDSPACES_API AFire : public ABuildableObject, public ISelectable
{
	GENERATED_BODY()

public:
	AFire();

	virtual void InitActions() override;
	void ExtinguishFire();
	
	virtual void OnSelected() override;
	virtual void OnDeselected() override;

	virtual bool StartUsingImplementation(ANPC* NPC) override;
	virtual bool StopUsingImplementation(ANPC* NPC) override;

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
};
