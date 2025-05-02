// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DXTitlePlayerController.generated.h"


UCLASS()
class SCC_DEDICATEDX_API ADXTitlePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

	void JoinServer(const FString& InIPAddress);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TSubclassOf<UUserWidget> UIWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TObjectPtr<UUserWidget> UIWidgetInstance;
};
