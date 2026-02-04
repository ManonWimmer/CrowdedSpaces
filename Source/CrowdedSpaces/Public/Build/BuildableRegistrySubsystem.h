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
	// Generators
	TArray<TWeakObjectPtr<ABuildableGenerator>> Generators;
	
	void RegisterGenerator(ABuildableGenerator* Generator) { Generators.Add(Generator); } // On Begin Play
	void UnregisterGenerator(ABuildableGenerator* Generator) { Generators.Remove(Generator); } // On Destroyed
	
	// Beds
	TArray<TWeakObjectPtr<ABuildableBed>> Beds;
	
	void RegisterBed(ABuildableBed* Bed) { Beds.Add(Bed); } // On Begin Play
	void UnregisterBed(ABuildableBed* Bed) { Beds.Remove(Bed); } // On Destroyed

	// Foods
	TArray<TWeakObjectPtr<ABuildableFood>> Foods;
	
	void RegisterFood(ABuildableFood* Food) { Foods.Add(Food); } // On Begin Play
	void UnregisterFood(ABuildableFood* Food) { Foods.Remove(Food); } // On Destroyed
};
