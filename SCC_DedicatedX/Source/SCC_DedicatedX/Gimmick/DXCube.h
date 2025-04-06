#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DXCube.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class FLifetimeProperty;
class UPointLightComponent;

UCLASS()
class SCC_DEDICATEDX_API ADXCube : public AActor
{
	GENERATED_BODY()
	
public:	
	ADXCube();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	virtual void Tick(float Deltaseconds) override;
	
	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;

	UFUNCTION()
	void OnRep_ServerRotationYaw();

	UFUNCTION()
	void OnRep_ServerLightColor();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPointLightComponent> PointLight;

	UPROPERTY(ReplicatedUsing = OnRep_ServerLightColor)
	FLinearColor ServerLightColor;

	UPROPERTY(ReplicatedUsing = OnRep_ServerRotationYaw)
	float ServerRotationYaw;

	float RotationSpeed;

	float NetUpdatePeriod;

	float AccDeltaSecondSinceReplicated;

	float NetCullDistance;
};
