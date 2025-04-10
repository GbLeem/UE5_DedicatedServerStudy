// Fill out your copyright notice in the Description page of Project Settings.
#include "Gimmick/DXLandMine.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

ADXLandMine::ADXLandMine()
	:bIsExploded(false)
	,NetCullDistance(1000.f)
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(SceneRoot);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneRoot);

	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Particle->SetupAttachment(SceneRoot);
	Particle->SetAutoActivate(false);

	SetNetCullDistanceSquared(NetCullDistance * NetCullDistance);
}

void ADXLandMine::BeginPlay()
{
	Super::BeginPlay();
	
	/*if (HasAuthority())
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Run on server")), true, true, FLinearColor::Green, 5.f);
	}
	else
	{
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		if (IsValid(OwnerPawn))
		{
			if (OwnerPawn->IsLocallyControlled())
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Run on owning client")), true, true, FLinearColor::Green, 5.f);
			}
			else
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Run on other client")), true, true, FLinearColor::Green, 5.f);
			}
		}
	}*/

	if (!OnActorBeginOverlap.IsAlreadyBound(this, &ThisClass::OnLandMineBeginOverlap))
	{
		OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnLandMineBeginOverlap);
	}
}

void ADXLandMine::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	//UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("ADXLandMine::EndPlay()")), true, true, FLinearColor::Green, 5.f);
	if (!OnActorBeginOverlap.IsAlreadyBound(this, &ThisClass::OnLandMineBeginOverlap))
	{
		OnActorBeginOverlap.RemoveDynamic(this, &ThisClass::OnLandMineBeginOverlap);
	}
}

void ADXLandMine::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsExploded);
}

void ADXLandMine::OnLandMineBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	//if server
	if (HasAuthority())
	{
		UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("run on server")), true, true, FLinearColor::Green, 5.f);
		MulticastRPCSpawnEffect();

		if (!bIsExploded)
		{
			bIsExploded = true;
		}
	}
	else
	{
		APawn* OwnerPawn = Cast<APawn>(GetOwner());
		if (IsValid(OwnerPawn))
		{
			if (OwnerPawn->IsLocallyControlled())
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Run on owning client")), true, true, FLinearColor::Green, 5.f);
			}
			else
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Run on other client")), true, true, FLinearColor::Green, 5.f);
			}
		}
		if (!bIsExploded)
		{
			Particle->Activate(true);
		}
	}
}

void ADXLandMine::OnRep_IsExploded()
{
	if (bIsExploded && IsValid(ExplodedMaterial))
	{
		Mesh->SetMaterial(0, ExplodedMaterial);
	}
}

void ADXLandMine::MulticastRPCSpawnEffect_Implementation()
{
	/*if (IsValid(ExplodedMaterial))
	{
		Mesh->SetMaterial(0, ExplodedMaterial);
	}*/
	//if (HasAuthority())
	//{
	//	bIsExploded = true;
	//	return;
	//}
	////already exploded
	//if (bIsExploded)
	//{
	//	return;
	//}
	//Particle->Activate(true);
	////bIsExploded = true;
}
