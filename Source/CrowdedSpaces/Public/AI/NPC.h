#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "Resources/ResourceComponent.h"
#include "Components/WidgetComponent.h"
#include "Selection/Selectable.h"
#include "NPCActionType.h"
#include "NPCPriorityType.h"
#include "TrainingSkillType.h"
#include "Production/ProductionType.h"
#include "NPC.generated.h"

enum class ETrainingSkillType : uint8;
enum class ENPCPriorityType : uint8;
class UBTTask_UseBuildableObject;
class ABuildableObject;
class USlotComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentActionChanged, ENPCActionType, Value); 

UCLASS()
class CROWDEDSPACES_API ANPC : public ACharacter, public ISelectable
{
	GENERATED_BODY()

public:
	ANPC();

	// Resource
	UFUNCTION(BlueprintCallable)
	UResourceComponent* GetResourceComponentByType(EResourceType Type) const;

	UFUNCTION(BlueprintCallable)
	int GetResourceByType(EResourceType Type) const;

	template <EResourceType Type>
	UResourceComponent* GetResourceComponent() const;

	template <EResourceType Type>
	int GetResource() const;

	// AI
	UBehaviorTree* GetBehaviorTree() const { return BehaviorTree; }
	
	UFUNCTION(BlueprintCallable, Category="AI")
	void Die();

	// Action
	UFUNCTION(BlueprintCallable, Category="AI")
	void SetCurrentAction(ENPCActionType NewAction);

	UFUNCTION(BlueprintCallable)
	ENPCActionType GetCurrentAction() const { return CurrentAction; }

	UPROPERTY(BlueprintAssignable)
	FOnCurrentActionChanged OnCurrentActionChanged;
	
	// Work
	UFUNCTION(BlueprintCallable, Category="AI")
	EProductionType GetWorkOnGeneratorType() const { return WorkOnGeneratorType; }

	UFUNCTION(BlueprintCallable, Category="AI")
	void SetWorkOnGeneratorType(EProductionType NewType);

	UFUNCTION(BlueprintCallable, Category="AI")
	int GetProductionMultiplierForType(EProductionType Type) const;
	
	void CancelCurrentUse() const;

	// Priority
	UFUNCTION(BlueprintCallable, Category="AI")
	ENPCPriorityType GetNPCPriorityType() const { return NPCPriorityType; }

	UFUNCTION(BlueprintCallable, Category="AI")
	void SetNPCPriorityType(ENPCPriorityType NewType);

	// Training
	UFUNCTION(BlueprintCallable, Category="AI")
	ETrainingSkillType GetTrainingSkillType() const { return TrainingSkillType; }

	UFUNCTION(BlueprintCallable, Category="AI")
	void SetTrainingSkillType(ETrainingSkillType NewType);
	
	// Name & Color
	UFUNCTION(BlueprintCallable, Category="AI")
	FString GetNPCName() const { return NPCName; }
	
	FTimerHandle NameRetryTimer;

	void TryGenerateName();

	static FLinearColor GetRandomColor();

	// Object
	UFUNCTION()
	void SetCurrentObject(ABuildableObject* NewObject);
	
	UFUNCTION()
	ABuildableObject* GetCurrentObject() const { return CurrentObject; }
	
protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

private:
	UPROPERTY()
	TMap<EResourceType, TObjectPtr<UResourceComponent>> ResourceMap;
	
	// Resources Components
	UPROPERTY(EditAnywhere)
	TObjectPtr<UResourceComponent> FoodComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UResourceComponent> EnergyComponent;

	// Work
	UPROPERTY(EditAnywhere)
	EProductionType WorkOnGeneratorType = EProductionType::Money;
	
	// Name
	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> NPCNameWidget;

	UPROPERTY()
	FString NPCName = "";
	
	// Action
	UPROPERTY(EditAnywhere)
	TObjectPtr<UWidgetComponent> NPCActionWidget;
	
	UPROPERTY()
	ENPCActionType CurrentAction = ENPCActionType::Idle;
	
	// Multipliers
	UPROPERTY()
	int FoodProductionMultiplier = 1;

	UPROPERTY()
	int ElectricityProductionMultiplier = 1;

	UPROPERTY()
	int MoneyProductionMultiplier = 1;

	// Priority
	UPROPERTY(EditAnywhere)
	ENPCPriorityType NPCPriorityType = ENPCPriorityType::Work;

	UPROPERTY(EditAnywhere)
	ETrainingSkillType TrainingSkillType = ETrainingSkillType::MoneyProduction;

	// Color
	UPROPERTY()
	UMaterialInstanceDynamic* BodyMaterialInstance;

	UPROPERTY()
	UMaterialInstanceDynamic* OtherMaterialInstance;

	// Object
	UPROPERTY()
	TObjectPtr<ABuildableObject> CurrentObject{nullptr};

	UPROPERTY()
	TObjectPtr<UBTTask_UseBuildableObject> CurrentUseTask{nullptr};
	
	// Selectable
public:
	virtual void OnSelected() override;
	virtual void OnDeselected() override;
};

template <EResourceType Type>
UResourceComponent* ANPC::GetResourceComponent() const
{
	return GetResourceComponentByType(Type);
}

template <EResourceType Type>
int ANPC::GetResource() const
{
	return GetResourceByType(Type);
}
