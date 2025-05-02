#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DXPlayerController.generated.h"

UCLASS()
class SCC_DEDICATEDX_API ADXPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const override;

public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FText NotificationText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> NotificationTextUIClass;
};
