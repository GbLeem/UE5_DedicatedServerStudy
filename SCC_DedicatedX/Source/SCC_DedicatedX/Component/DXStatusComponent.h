#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DXStatusComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentHPChangedDelegate, float);
DECLARE_MULTICAST_DELEGATE(FOnOutOfCurrentHPDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMaxHPChangedDelegate, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SCC_DEDICATEDX_API UDXStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDXStatusComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	float ApplyDamage(float InDamage);

	float GetCurrentHP()const { return CurrentHP; }
	void SetCurrentHP(float InCurrentHP);

	float GetMaxHP() const { return MaxHP; }
	void SetMaxHP(float InMaxHP);


protected:
	UFUNCTION()
	void OnRep_CurrentHP();

	UFUNCTION()
	void OnRep_MaxHP();

public:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHP)
	float CurrentHP;

	UPROPERTY(ReplicatedUsing = OnRep_MaxHP)
	float MaxHP;

	FOnCurrentHPChangedDelegate OnCurrentHPChanged;

	FOnOutOfCurrentHPDelegate OnOutOfCurrentHP;

	FOnMaxHPChangedDelegate OnMaxHPChanged;
};
