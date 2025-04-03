#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DXPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SCC_DEDICATEDX_API ADXPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADXPlayerController();

	virtual void PostInitializeComponents() override;

	virtual void PostNetInit() override;

	virtual void BeginPlay() override; 

	virtual void OnPossess(APawn* InPawn) override;
};
