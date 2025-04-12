#pragma once

#include "CoreMinimal.h"
#include "DXCharacterBase.h"
#include "InputActionValue.h"
#include "DXPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UDXStatusComponent;
class UDXHPTextWidgetComponent;
class UUW_HPText;

UCLASS()
class SCC_DEDICATEDX_API ADXPlayerCharacter : public ADXCharacterBase
{
	GENERATED_BODY()

public:
	ADXPlayerCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Tick(float DeltaTime);
#pragma region Overrides Character

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void BeginPlay() override;

#pragma endregion

#pragma region DXPlayerCharacter Components

public:
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }

	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DXPlayerCharacter|Components")
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DXPlayerCharacter|Components")
	TObjectPtr<UCameraComponent> Camera;

#pragma endregion

#pragma region Input
private:
	void HandleMoveInput(const FInputActionValue& InValue);

	void HandleLookInput(const FInputActionValue& InValue);

	void HandleLandMineInput(const FInputActionValue& InValue);

	void HandleMeleeAttackInput(const FInputActionValue& InValue);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DXPlayerCharacter|Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DXPlayerCharacter|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DXPlayerCharacter|Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DXPlayerCharacter|Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DXPlayerCharacter|Input")
	TObjectPtr<UInputAction> LandMineAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DXPlayerCharacter|Input")
	TObjectPtr<UInputAction> MeleeAttackAction;

#pragma endregion

#pragma region LandMine
private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCSpawnLandMine();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> LandMineClass;

#pragma endregion

#pragma region Attack

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void CheckMeleeAttackHit();

private:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDrawDebugMeleeAttack(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCMeleeAttack(float InStartMeleeAttackTime);

	UFUNCTION(NetMulticast, UnReliable)
	void MulticastRPCMeleeAttack();

	UFUNCTION()
	void OnRep_CanAttack();

	void PlayMeleeAttackMontage();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPCPerformMeleeHit(ACharacter* InDamagedCharacters, float InCheckTime);

	UFUNCTION(Client, Unreliable)
	void ClientRPCPlayMeleeAttackMontage(ADXPlayerCharacter* InTargetCharacter);

protected:
	UPROPERTY(ReplicatedUsing = OnRep_CanAttack)
	uint8 bCanAttack : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> MeleeAttackMontage;
	
	float MeleeAttackMontagePlayTime;

	float LastStartMeleeAttackTime;

	float MeleeAttackTimeDifference;

	float MinAllowedTimeForMeleeAttack;
#pragma endregion

#pragma region DXPlayerCharacter Components
public:

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DXPlayerCharacter|Components")
	TObjectPtr<UDXStatusComponent> StatusComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DXPlayerCharacter|Components")
	TObjectPtr<UDXHPTextWidgetComponent> HPTextWidgetComponent;

#pragma endregion

#pragma region HPWidget
public:
	void SetHPTextWidget(UUW_HPText* InHPTextWidget);

#pragma endregion
};
