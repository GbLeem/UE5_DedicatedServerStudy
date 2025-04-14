#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DXBuffBox.generated.h"

class UBoxComponent;

UCLASS()
class SCC_DEDICATEDX_API ADXBuffBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ADXBuffBox();

	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnEffectFinished(UParticleSystemComponent* ParticleSystem);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Box")
	TObjectPtr<UBoxComponent> TriggerBox;

	UPROPERTY(VisibleAnywhere, Category = "Box")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TObjectPtr<UParticleSystemComponent> ParticleEffect;

};
