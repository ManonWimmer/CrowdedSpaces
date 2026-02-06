#pragma once

#include "CoreMinimal.h"
#include "GridWallEdge.generated.h"

UENUM()
enum class EGridWallDirection : uint8
{
	North,
	East,
	South,
	West
};

USTRUCT()
struct FGridWallEdge
{
	GENERATED_BODY()

	FIntPoint Cell;
	EGridWallDirection Direction;

	bool operator==(const FGridWallEdge& Other) const
	{
		return Cell == Other.Cell && Direction == Other.Direction;
	}
};

FORCEINLINE uint32 GetTypeHash(const FGridWallEdge& Edge)
{
	return HashCombine(GetTypeHash(Edge.Cell), GetTypeHash(static_cast<uint8>(Edge.Direction)));
}
