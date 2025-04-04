#include "Gimmick/DXCube.h"

#include "SCC_DedicatedX.h"
#include "Net/UnrealNetwork.h"

ADXCube::ADXCube()
	:ServerRotationYaw(0.f)
	,RotationSpeed(30.f)
	,AccDeltaSecondSinceReplicated(0.f)
{ 
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(SceneRoot);
	Mesh->SetRelativeLocation(FVector(-50.f, -50.f, 50.f));

	const static float CubeActorNetUpdateFrequency = 1.f;
	SetNetUpdateFrequency(CubeActorNetUpdateFrequency);
	NetUpdatePeriod = CubeActorNetUpdateFrequency / GetNetUpdateFrequency();
}

void ADXCube::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ServerRotationYaw);
}

void ADXCube::Tick(float Deltaseconds)
{
	Super::Tick(Deltaseconds);

	if (HasAuthority())
	{
		AddActorLocalRotation(FRotator(0.f, RotationSpeed * Deltaseconds, 0.f));
		ServerRotationYaw = RootComponent->GetComponentRotation().Yaw;
	}
	else
	{
		if (NetUpdatePeriod < KINDA_SMALL_NUMBER)
		{
			return;
		}
		AccDeltaSecondSinceReplicated += Deltaseconds;
		const float LerpRatio = FMath::Clamp(AccDeltaSecondSinceReplicated / NetUpdateFrequency, 0.f, 1.f);
		const float NextServerRotationYaw = ServerRotationYaw + RotationSpeed * NetUpdatePeriod;
		const float EstimatedClientRotationYaw = FMath::Lerp(ServerRotationYaw, NextServerRotationYaw, LerpRatio);
		SetActorRotation(FRotator(0.f, EstimatedClientRotationYaw, 0.f));		
	}
}

void ADXCube::OnRep_ServerRotationYaw()
{
	SetActorRotation(FRotator(0.f, ServerRotationYaw, 0.f));
	AccDeltaSecondSinceReplicated = 0.f;
}


