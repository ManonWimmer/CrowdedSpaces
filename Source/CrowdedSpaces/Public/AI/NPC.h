#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPC.generated.h"

UCLASS()
class CROWDEDSPACES_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:
	ANPC();

protected:
	virtual void BeginPlay() override;
};
