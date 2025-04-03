#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DXCharacterBase.generated.h"

UCLASS()
class SCC_DEDICATEDX_API ADXCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ADXCharacterBase();

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_Owner() override;

	virtual void PostNetInit() override;
};
