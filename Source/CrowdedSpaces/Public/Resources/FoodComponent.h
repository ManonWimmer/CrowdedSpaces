#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Selection/SelectableStatProvider.h"
#include "FoodComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CROWDEDSPACES_API UFoodComponent : public UActorComponent, public ISelectableStatProvider
{
	GENERATED_BODY()

public:
	UFoodComponent();

	UFUNCTION(BlueprintCallable)
	void AddFood(int Amount);

	UFUNCTION(BlueprintCallable)
	void RemoveFood(int Amount);

	UFUNCTION(BlueprintCallable)
	bool HasEnoughFood(int Amount);

	UFUNCTION(BlueprintCallable)
	int GetFood() {return Food; }


	// Selectable
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnStatChanged;
	
	virtual TArray<TPair<FString, FString>> GetCurrentValues() const override;
	virtual FOnStatChanged& GetOnStatChanged() override { return OnStatChanged; }
	
private:
	UPROPERTY(EditAnywhere)
	int32 Food = 0;
};
