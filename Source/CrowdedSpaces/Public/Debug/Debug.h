#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoralEvent/MoralEvent.h"
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
	void StartDebugMoralEvent() const;

private:
	UPROPERTY(EditAnywhere, Category = "Debug")
	TSubclassOf<UMoralEvent> DebugEventData;
};
