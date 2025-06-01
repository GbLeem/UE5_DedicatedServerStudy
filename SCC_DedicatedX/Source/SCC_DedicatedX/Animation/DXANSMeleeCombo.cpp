// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DXANSMeleeCombo.h"

#include "Character/DXPlayerCharacter.h"

void UDXANSMeleeCombo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	ADXPlayerCharacter* PlayerCharacter = Cast<ADXPlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->SetNextComboAttack(SectionName);
		//PlayerCharacter->bCanCombo = false;
	}
}

void UDXANSMeleeCombo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	ADXPlayerCharacter* PlayerCharacter = Cast<ADXPlayerCharacter>(MeshComp->GetOwner());
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->ResetComboAttack();
	}
}
