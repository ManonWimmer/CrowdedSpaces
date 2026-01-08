#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Debug.generated.h"

UCLASS()
class CROWDEDSPACES_API ADebug : public AActor
{
	GENERATED_BODY()

public:
	ADebug();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(CallInEditor, Category = "Debug")
	void Debug_SetGameMode_Game() const;

	UFUNCTION(CallInEditor, Category = "Debug")
	void Debug_SetGameMode_Building() const;
};
