#pragma once

#include "CoreMinimal.h"
#include "Object/Buildable/BuildableBed.h"
#include "Object/Buildable/BuildableFood.h"
#include "Object/Buildable/BuildableGenerator.h"
#include "Object/Buildable/BuildableTrainingStation.h"
#include "Subsystems/WorldSubsystem.h"
#include "BuildableRegistrySubsystem.generated.h"

UCLASS()
class CROWDEDSPACES_API UBuildableRegistrySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// All
	TArray<TWeakObjectPtr<AUsableObject>> BuildableObjects;
	
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

	// Training Stations
	TArray<TWeakObjectPtr<ABuildableTrainingStation>> TrainingStations;
	
	void RegisterTrainingStation(ABuildableTrainingStation* TrainingStation);
	void UnregisterTrainingStation(ABuildableTrainingStation* TrainingStation); 
};
