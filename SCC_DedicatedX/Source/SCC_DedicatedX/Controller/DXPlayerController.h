#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DXPlayerController.generated.h"

class UUW_GameResult;

UCLASS()
class SCC_DEDICATEDX_API ADXPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

	void OnCharaterDead();

	UFUNCTION(Client, Reliable)
	void ClientRPCShowGameResultWidget(int32 InRanking);

	UFUNCTION(Client, Reliable)
	void ClientRPCReturnToTitle();

public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FText NotificationText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> NotificationTextUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUW_GameResult> GameResultUIClass;
};
