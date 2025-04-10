#include "Character/DXPlayerCharacter.h"

#include "Gimmick/DXLandMine.h"

#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetSystemLibrary.h"

ADXPlayerCharacter::ADXPlayerCharacter()
	:bCanAttack(true)
	,MeleeAttackMontagePlayTime(0.f)
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->bUsePawnControlRotation = false;
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

void ADXPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::HandleMoveInput);
	
	EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::HandleLookInput);

	EIC->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ThisClass::Jump);
	EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ThisClass::StopJumping);

	EIC->BindAction(LandMineAction, ETriggerEvent::Started, this, &ThisClass::HandleLandMineInput);

	EIC->BindAction(MeleeAttackAction, ETriggerEvent::Started, this, &ThisClass::HandleMeleeAttackInput);
}

void ADXPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority() && IsLocallyControlled())
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		checkf(IsValid(PC), TEXT("PlayerController is invalid"));

		UEnhancedInputLocalPlayerSubsystem* EILPS = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
		checkf(IsValid(EILPS), TEXT("EnhancedInputLocalPlayerSubsystem is invalid"));

		EILPS->AddMappingContext(InputMappingContext, 0);
	}

	if (IsValid(MeleeAttackMontage))
	{
		MeleeAttackMontagePlayTime = MeleeAttackMontage->GetPlayLength();
	}
}

void ADXPlayerCharacter::HandleMoveInput(const FInputActionValue& InValue)
{
	if (!IsValid(Controller))
	{
		UE_LOG(LogTemp, Error, TEXT("Controller is invalid."));
		return;
	}

	const FVector2D InMovementVector = InValue.Get<FVector2D>();

	const FRotator ControlRotation = Controller->GetControlRotation();
	const FRotator ControlYawRotation(0.0f, ControlRotation.Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(ControlYawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, InMovementVector.X);
	AddMovementInput(RightDirection, InMovementVector.Y);
}

void ADXPlayerCharacter::HandleLookInput(const FInputActionValue& InValue)
{
	if (!IsValid(Controller))
	{
		UE_LOG(LogTemp, Error, TEXT("Controller is invalid."));
		return;
	}

	const FVector2D InLookVector = InValue.Get<FVector2D>();

	AddControllerYawInput(InLookVector.X);
	AddControllerPitchInput(InLookVector.Y);
}

void ADXPlayerCharacter::HandleLandMineInput(const FInputActionValue& InValue)
{	
	if (!HasAuthority() && IsLocallyControlled())
	{
		ServerRPCSpawnLandMine();
	}
}

void ADXPlayerCharacter::HandleMeleeAttackInput(const FInputActionValue& InValue)
{
	if (bCanAttack && !GetCharacterMovement()->IsFalling())
	{
		bCanAttack = false;

		GetCharacterMovement()->SetMovementMode(MOVE_None);
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda
		([&]() -> void
			{
				bCanAttack = true;
				GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			}), MeleeAttackMontagePlayTime, false
		);

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (IsValid(AnimInstance))
		{
			AnimInstance->Montage_Play(MeleeAttackMontage);
		}
	}
}

float ADXPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("TakeDamage: %f"), DamageAmount), true, true, FLinearColor::Red, 5.f);

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ADXPlayerCharacter::CheckMeleeAttackHit()
{
	TArray<FHitResult> OutHitResults;
	TSet<ACharacter*> DamagedCharacters;
	FCollisionQueryParams Params(NAME_None, false, this);

	const float MeleeAttackRange = 50.f;
	const float MeleeAttackRadius = 50.f;
	const float MeleeAttackDamage = 10.f;

	const FVector Forward = GetActorForwardVector();
	const FVector Start = GetActorLocation() + Forward * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * MeleeAttackRange;

	bool bIsHitDetected = GetWorld()->SweepMultiByChannel(OutHitResults, Start, End, FQuat::Identity, ECC_Camera, FCollisionShape::MakeSphere(MeleeAttackRadius), Params);
	if (bIsHitDetected)
	{
		for (auto const& OutHitResult : OutHitResults)
		{
			ACharacter* DamagedCharacter = Cast<ACharacter>(OutHitResult.GetActor());
			if (IsValid(DamagedCharacter))
			{
				DamagedCharacters.Add(DamagedCharacter);
			}
		}

		FDamageEvent DamageEvent;
		for (auto const& DamagedCharacter : DamagedCharacters)
		{
			DamagedCharacter->TakeDamage(MeleeAttackDamage, DamageEvent, GetController(), this);
		}
	}
	FColor DrawColor = bIsHitDetected ? FColor::Green : FColor::Red;
	DrawDebugMeleeAttack(DrawColor, Start, End, Forward);
}

void ADXPlayerCharacter::DrawDebugMeleeAttack(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward)
{
	const float MeleeAttackRange = 50.f;
	const float MeleeAttackRadius = 50.f;
	FVector CapsuleOrigin = TraceStart + (TraceEnd - TraceStart) * 0.5f;
	float CapsuleHalfHeight = MeleeAttackRange * 0.5f;
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, MeleeAttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.f);
}

void ADXPlayerCharacter::ServerRPCSpawnLandMine_Implementation()
{
	if (IsValid(LandMineClass))
	{
		FVector SpawnedLocation = (GetActorLocation() + GetActorForwardVector() * 300.f) - FVector(0.f, 0.f, 90.f);
		ADXLandMine* SpawnedLandMine = GetWorld()->SpawnActor<ADXLandMine>(LandMineClass, SpawnedLocation, FRotator::ZeroRotator);

		SpawnedLandMine->SetOwner(this);
	}
}

bool ADXPlayerCharacter::ServerRPCSpawnLandMine_Validate()
{
	return true;
}