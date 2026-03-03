#include "AI/NameGeneratorSubsystem.h"

void UNameGeneratorSubsystem::SetNameData(UNameList* NewNameList)
{
	NameData = NewNameList;
	bIsInitialized = true;
}

FString UNameGeneratorSubsystem::GenerateName() const
{
	if (!NameData || NameData->FirstNames.Num() == 0 || NameData->LastNames.Num() == 0)
		return "NPC";

	const int32 FirstIndex = FMath::RandRange(0, NameData->FirstNames.Num() - 1);
	const int32 LastIndex  = FMath::RandRange(0, NameData->LastNames.Num() - 1);

	return NameData->FirstNames[FirstIndex] + " " + NameData->LastNames[LastIndex];
}
