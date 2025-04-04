#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DXCube.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class FLifetimeProperty;

UCLASS()
class SCC_DEDICATEDX_API ADXCube : public AActor
{
	GENERATED_BODY()
	
public:	
	ADXCube();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float Deltaseconds) override;
	
	UFUNCTION()
	void OnRep_ServerRotationYaw();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(ReplicatedUsing = OnRep_ServerRotationYaw)
	float ServerRotationYaw;

	float RotationSpeed;

	float NetUpdatePeriod;

	float AccDeltaSecondSinceReplicated;
};
