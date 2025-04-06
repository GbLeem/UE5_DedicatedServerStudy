// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DXLandMine.generated.h"

class USceneComponent;
class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class SCC_DEDICATEDX_API ADXLandMine : public AActor
{
	GENERATED_BODY()
	
public:	
	ADXLandMine();

protected:
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnLandMineBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastRPCSpawnEffect();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess))
	TObjectPtr<UParticleSystemComponent> Particle;

	bool bIsExploded;
};
