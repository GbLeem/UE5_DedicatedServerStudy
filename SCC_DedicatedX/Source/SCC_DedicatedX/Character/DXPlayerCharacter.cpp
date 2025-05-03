#include "Character/DXPlayerCharacter.h"

#include "Gimmick/DXLandMine.h"
#include "Controller/DXPlayerController.h"
#include "GameState/DXGameStateBase.h"
#include "Component/DXStatusComponent.h"
#include "Component/DXHPTextWidgetComponent.h"
#include "UI/UW_HPText.h"

#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/DamageEvents.h"
#include "EngineUtils.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/GameStateBase.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

ADXPlayerCharacter::ADXPlayerCharacter()
	:bCanAttack(true)
	,MeleeAttackMontagePlayTime(0.f)
	,LastStartMeleeAttackTime(0.f)
	,MeleeAttackTimeDifference(0.f)
	, MinAllowedTimeForMeleeAttack(0.02f)
{
	PrimaryActorTick.bCanEverTick = true;

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

	//status and ui
	StatusComponent = CreateDefaultSubobject<UDXStatusComponent>(TEXT("StatusComponent"));

	HPTextWidgetComponent = CreateDefaultSubobject<UDXHPTextWidgetComponent>(TEXT("HPTextWidgetComponent"));
	HPTextWidgetComponent->SetupAttachment(GetRootComponent());
	HPTextWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f));

	HPTextWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	HPTextWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ADXPlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bCanAttack);
}

void ADXPlayerCharacter::Tick(float DeltaTime)
{
	if (IsValid(HPTextWidgetComponent) && !HasAuthority())
	{
		FVector WidgetComponentLocation = HPTextWidgetComponent->GetComponentLocation();
		FVector LocalPlayerCameraLocation = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();
		HPTextWidgetComponent->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(WidgetComponentLocation, LocalPlayerCameraLocation));
	}
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

	StatusComponent->OnOutOfCurrentHP.AddUObject(this, &ThisClass::OnDeath);
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
		//ServerRPCMeleeAttack();
		ServerRPCMeleeAttack(GetWorld()->GetGameState()->GetServerWorldTimeSeconds());

		if (!HasAuthority() && IsLocallyControlled())
		{
			PlayMeleeAttackMontage();
		}
	}
}

float ADXPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("TakeDamage: %f"), DamageAmount), true, true, FLinearColor::Red, 5.f);

	//return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	ADXGameStateBase* DXGameState = Cast<ADXGameStateBase>(UGameplayStatics::GetGameState(this));
	if (IsValid(DXGameState) && DXGameState->MatchState == EMatchState::Playing)
	{
		StatusComponent->ApplyDamage(ActualDamage);
	}
	return ActualDamage;
}

void ADXPlayerCharacter::CheckMeleeAttackHit()
{
	//server check
	//if (HasAuthority())
	if(IsLocallyControlled())
	{
		TArray<FHitResult> OutHitResults;
		TSet<ACharacter*> DamagedCharacters;
		FCollisionQueryParams Params(NAME_None, false, this);

		const float MeleeAttackRange = 50.f;
		const float MeleeAttackRadius = 50.f;
		//const float MeleeAttackDamage = 10.f;

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
				//DamagedCharacter->TakeDamage(MeleeAttackDamage, DamageEvent, GetController(), this);
				ServerRPCPerformMeleeHit(DamagedCharacter, GetWorld()->GetGameState()->GetServerWorldTimeSeconds());
			}
		}
		FColor DrawColor = bIsHitDetected ? FColor::Green : FColor::Red;
		MulticastDrawDebugMeleeAttack(DrawColor, Start, End, Forward);
	}
}

void ADXPlayerCharacter::ServerRPCMeleeAttack_Implementation(float InStartMeleeAttackTime)
{
	MeleeAttackTimeDifference = GetWorld()->GetTimeSeconds() - InStartMeleeAttackTime;
	MeleeAttackTimeDifference = FMath::Clamp(MeleeAttackTimeDifference, 0.f, MeleeAttackMontagePlayTime);

	if (KINDA_SMALL_NUMBER < MeleeAttackMontagePlayTime - MeleeAttackTimeDifference)
	{
		bCanAttack = false;
		OnRep_CanAttack();

		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda
		([&]() -> void
			{
				bCanAttack = true;
				OnRep_CanAttack();
			}), MeleeAttackMontagePlayTime - MeleeAttackTimeDifference, false, -1.f
		);
	}
	LastStartMeleeAttackTime = InStartMeleeAttackTime;

	PlayMeleeAttackMontage();

	//MulticastRPCMeleeAttack();
	for (APlayerController* PlayerController : TActorRange<APlayerController>(GetWorld())) //controller of the attacking player
	{		
		if (IsValid(PlayerController) && GetController() != PlayerController) //except attaker
		{
			ADXPlayerCharacter* OtherPlayerCharacter = Cast<ADXPlayerCharacter>(PlayerController->GetPawn());
			if(IsValid(OtherPlayerCharacter))
			{
				OtherPlayerCharacter->ClientRPCPlayMeleeAttackMontage(this);
			}
		}
	}
}

bool ADXPlayerCharacter::ServerRPCMeleeAttack_Validate(float InStartMeleeAttackTime)
{
	if (LastStartMeleeAttackTime == 0.f)
	{
		return true;
	}
	return (MeleeAttackMontagePlayTime - 0.1f) < (InStartMeleeAttackTime - LastStartMeleeAttackTime);
}

void ADXPlayerCharacter::MulticastDrawDebugMeleeAttack_Implementation(const FColor& DrawColor, FVector TraceStart, FVector TraceEnd, FVector Forward)
{
	const float MeleeAttackRange = 50.f;
	const float MeleeAttackRadius = 50.f;
	FVector CapsuleOrigin = TraceStart + (TraceEnd - TraceStart) * 0.5f;
	float CapsuleHalfHeight = MeleeAttackRange * 0.5f;
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, MeleeAttackRadius, FRotationMatrix::MakeFromZ(Forward).ToQuat(), DrawColor, false, 5.f);
}

void ADXPlayerCharacter::MulticastRPCMeleeAttack_Implementation()
{
	if (!HasAuthority() && !IsLocallyControlled())
	{
		PlayMeleeAttackMontage();
	}
}

void ADXPlayerCharacter::OnRep_CanAttack()
{
	if (bCanAttack)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	else
	{
		GetCharacterMovement()->SetMovementMode(MOVE_None);
	}
}

void ADXPlayerCharacter::PlayMeleeAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->StopAllMontages(0.f);
		AnimInstance->Montage_Play(MeleeAttackMontage);
	}
}

void ADXPlayerCharacter::OnDeath()
{
	ADXPlayerController* PlayerController = GetController<ADXPlayerController>();
	//server
	if (IsValid(PlayerController) && HasAuthority())
	{
		PlayerController->OnCharaterDead();
	}
}

void ADXPlayerCharacter::SetHPTextWidget(UUW_HPText* InHPTextWidget)
{
	UUW_HPText* HPWidget = Cast<UUW_HPText>(InHPTextWidget);
	if (IsValid(HPWidget))
	{
		HPWidget->InitializeHPTextWidget(StatusComponent);
		StatusComponent->OnCurrentHPChanged.AddUObject(HPWidget, &UUW_HPText::OnCurrentHPChange);
		StatusComponent->OnMaxHPChanged.AddUObject(HPWidget, &UUW_HPText::OnMaxHPChange);
	}
}

void ADXPlayerCharacter::TakeBuff(float InBuffValue)
{
	if (IsValid(StatusComponent))
	{
		StatusComponent->SetMaxHP(StatusComponent->GetMaxHP() + InBuffValue);
		StatusComponent->SetCurrentHP(StatusComponent->GetCurrentHP() + InBuffValue);
	}
}

void ADXPlayerCharacter::ClientRPCPlayMeleeAttackMontage_Implementation(ADXPlayerCharacter* InTargetCharacter)
{
	if (IsValid(InTargetCharacter))
	{
		InTargetCharacter->PlayMeleeAttackMontage();
	}
}

void ADXPlayerCharacter::ServerRPCPerformMeleeHit_Implementation(ACharacter* InDamagedCharacters, float InCheckTime)
{
	/*for (auto const& InHitResult : InHitResults)
	{
		AActor* HitActor = InHitResult.GetActor();
		if (IsValid(HitActor))
		{
			const float MeleeAttackDamage = 10.f;
			FDamageEvent DamageEvent;
			HitActor->TakeDamage(MeleeAttackDamage, DamageEvent, GetController(), this);
		}
	}*/
	if (IsValid(InDamagedCharacters))
	{		
		const float MeleeAttackDamage = 10.f;
		FDamageEvent DamageEvent;		
		InDamagedCharacters->TakeDamage(MeleeAttackDamage, DamageEvent, GetController(), this);
	}
}

bool ADXPlayerCharacter::ServerRPCPerformMeleeHit_Validate(ACharacter* InDamagedCharacters, float InCheckTime)
{
	return MinAllowedTimeForMeleeAttack < (InCheckTime - LastStartMeleeAttackTime);
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