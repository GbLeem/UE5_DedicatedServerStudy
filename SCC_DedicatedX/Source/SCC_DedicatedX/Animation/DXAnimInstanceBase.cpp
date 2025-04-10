#include "Animation/DXAnimInstanceBase.h"

#include "Character/DXPlayerCharacter.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UDXAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<ACharacter>(GetOwningActor());
	if (IsValid(OwnerCharacter))
	{
		OwnerCharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void UDXAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(OwnerCharacter) || !IsValid(OwnerCharacterMovementComponent))
	{
		return;
	}
	Velocity = OwnerCharacterMovementComponent->Velocity;
	GroundSpeed = FVector(Velocity.X, Velocity.Y, 0.f).Size();
	bShouldMove = (!(OwnerCharacterMovementComponent->GetCurrentAcceleration().IsNearlyZero()) && (3.f < GroundSpeed));
	bIsFalling = OwnerCharacterMovementComponent->IsFalling();
}

void UDXAnimInstanceBase::AnimNotify_CheckMeleeAttackHit()
{
	ADXPlayerCharacter* OwnerPlayerCharacter = Cast<ADXPlayerCharacter>(OwnerCharacter);
	if (IsValid(OwnerPlayerCharacter))
	{
		OwnerPlayerCharacter->CheckMeleeAttackHit();
	}
}
