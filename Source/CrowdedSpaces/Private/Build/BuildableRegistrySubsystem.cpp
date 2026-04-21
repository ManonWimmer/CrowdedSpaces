#include "Build/BuildableRegistrySubsystem.h"

#pragma region Generator
void UBuildableRegistrySubsystem::RegisterGenerator(ABuildableGenerator* Generator)
{
	Generators.Add(Generator);
	BuildableObjects.Add(Generator);
}

void UBuildableRegistrySubsystem::UnregisterGenerator(ABuildableGenerator* Generator)
{
	Generators.Remove(Generator);
	BuildableObjects.Remove(Generator);
}
#pragma endregion

#pragma region Bed
void UBuildableRegistrySubsystem::RegisterBed(ABuildableBed* Bed)
{
	Beds.Add(Bed);
	BuildableObjects.Add(Bed);
}

void UBuildableRegistrySubsystem::UnregisterBed(ABuildableBed* Bed)
{
	Beds.Remove(Bed);
	BuildableObjects.Remove(Bed);
}
#pragma endregion

#pragma region Food
void UBuildableRegistrySubsystem::RegisterFood(ABuildableFood* Food)
{
	Foods.Add(Food);
	BuildableObjects.Add(Food);
}

void UBuildableRegistrySubsystem::UnregisterFood(ABuildableFood* Food)
{
	Foods.Remove(Food);
	BuildableObjects.Remove(Food);
}
#pragma endregion

#pragma region Training Station
void UBuildableRegistrySubsystem::RegisterTrainingStation(ABuildableTrainingStation* TrainingStation)
{
	TrainingStations.Add(TrainingStation);
	BuildableObjects.Add(TrainingStation);
}

void UBuildableRegistrySubsystem::UnregisterTrainingStation(ABuildableTrainingStation* TrainingStation)
{
	TrainingStations.Remove(TrainingStation);
	BuildableObjects.Remove(TrainingStation);
}
#pragma endregion
