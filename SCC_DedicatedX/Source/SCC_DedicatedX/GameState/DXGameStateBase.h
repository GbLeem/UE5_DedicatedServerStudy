#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DXGameStateBase.generated.h"


UCLASS()
class SCC_DEDICATEDX_API ADXGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ADXGameStateBase();

	virtual void HandleBeginPlay() override;

	virtual void OnRep_ReplicatedHasBegunPlay() override;
};
