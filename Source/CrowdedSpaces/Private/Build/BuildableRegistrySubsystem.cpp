// Fill out your copyright notice in the Description page of Project Settings.


#include "Build/BuildableRegistrySubsystem.h"

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
