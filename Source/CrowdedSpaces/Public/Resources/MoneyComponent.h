#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Selection/SelectableStatProvider.h"
#include "MoneyComponent.generated.h"

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
	int GetMoney() {return Money; }

	// Selectable
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnStatChanged;
	
	virtual TArray<TPair<FString, FString>> GetCurrentValues() const override;
	virtual FOnStatChanged& GetOnStatChanged() override { return OnStatChanged; }
	
private:
	UPROPERTY(EditAnywhere)
	int32 Money = 0;
};
