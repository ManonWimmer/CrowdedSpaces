#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoralEvent/MoralEventData.h"
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
	void SetGameMode_Game() const;

	UFUNCTION(CallInEditor, Category = "Debug")
	void SetGameMode_Building() const;

	UFUNCTION(CallInEditor, Category = "Debug")
	void StartDebugEventData() const;

private:
	UPROPERTY(EditAnywhere, Category = "Debug")
	UMoralEventData* DebugEventData;
};
