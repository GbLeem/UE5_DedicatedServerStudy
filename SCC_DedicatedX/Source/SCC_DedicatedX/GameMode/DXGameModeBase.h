#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DXGameModeBase.generated.h"


UCLASS()
class SCC_DEDICATEDX_API ADXGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADXGameModeBase();

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)override;

	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void StartPlay() override;
};
