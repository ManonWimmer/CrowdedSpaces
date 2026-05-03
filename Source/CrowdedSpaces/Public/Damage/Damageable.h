#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

UINTERFACE(Blueprintable)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

class CROWDEDSPACES_API IDamageable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TakeDamage(const float Amount);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Heal(const float Amount);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartHeal(const float RegenAmountPerTick);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EndHeal();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float GetMaxHealth() const;

	UFUNCTION()
	virtual void OnDamaged() = 0;
	
	UFUNCTION()
	virtual void OnHealed() = 0;

	UFUNCTION()
	virtual void OnDead() = 0;
};
