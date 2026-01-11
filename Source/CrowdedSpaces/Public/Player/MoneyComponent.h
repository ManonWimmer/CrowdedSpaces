#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoneyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, NewAmount);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWDEDSPACES_API UMoneyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMoneyComponent();
	
	UFUNCTION(BlueprintCallable)
	void AddMoney(int Amount);

	UFUNCTION(BlueprintCallable)
	void RemoveMoney(int Amount);

	UFUNCTION(BlueprintCallable)
	bool HasEnoughMoney(int Amount);

	UFUNCTION(BlueprintCallable)
	int GetMoney() {return Money; }

	UPROPERTY(BlueprintAssignable)
	FOnMoneyChanged OnMoneyChanged;
	
private:
	UPROPERTY(EditAnywhere)
	int32 Money = 0;
};
