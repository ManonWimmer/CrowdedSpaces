#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Selection/SelectableStatProvider.h"
#include "MoneyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, NewValue);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWDEDSPACES_API UMoneyComponent : public UActorComponent, public ISelectableStatProvider
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
	int GetMoney() const {return Money; }

	UPROPERTY(BlueprintAssignable, Category = "Money")
	FOnMoneyChanged OnMoneyChanged;

	// Selectable
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnStatChanged;
	
	virtual TArray<FStat> GetCurrentValues() const override;
	virtual FOnStatChanged& GetOnStatChanged() override { return OnStatChanged; }
	
private:
	UPROPERTY(EditAnywhere)
	int32 Money = 0;
};
