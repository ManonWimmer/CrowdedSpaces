#pragma once

#include "CoreMinimal.h"
#include "Buildable/BuildableBed.h"
#include "Buildable/BuildableFood.h"
#include "Buildable/BuildableGenerator.h"
#include "Subsystems/WorldSubsystem.h"
#include "BuildableRegistrySubsystem.generated.h"

UCLASS()
class CROWDEDSPACES_API UBuildableRegistrySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// All
	TArray<TWeakObjectPtr<ABuildableObject>> BuildableObjects;
	
	// Generators
	TArray<TWeakObjectPtr<ABuildableGenerator>> Generators;
	
	void RegisterGenerator(ABuildableGenerator* Generator);
	void UnregisterGenerator(ABuildableGenerator* Generator); 
	
	// Beds
	TArray<TWeakObjectPtr<ABuildableBed>> Beds;
	
	void RegisterBed(ABuildableBed* Bed);
	void UnregisterBed(ABuildableBed* Bed);

	// Foods
	TArray<TWeakObjectPtr<ABuildableFood>> Foods;
	
	void RegisterFood(ABuildableFood* Food);
	void UnregisterFood(ABuildableFood* Food);
};
