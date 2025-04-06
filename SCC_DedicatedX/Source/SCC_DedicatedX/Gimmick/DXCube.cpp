#include "Gimmick/DXCube.h"

#include "SCC_DedicatedX.h"
#include "Net/UnrealNetwork.h"
#include "Components/PointLightComponent.h"

ADXCube::ADXCube()
	:ServerRotationYaw(0.f)
	,RotationSpeed(30.f)
	,AccDeltaSecondSinceReplicated(0.f)
	,NetCullDistance(1000.f)
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

	//net cull distance
	SetNetCullDistanceSquared(NetCullDistance * NetCullDistance);

	//net dormancy
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(SceneRoot);

	SetNetDormancy(DORM_Initial);
}

void ADXCube::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ServerRotationYaw);
	DOREPLIFETIME(ThisClass, ServerLightColor);
	//DOREPLIFETIME_CONDITION(ThisClass, ServerLightColor, COND_InitialOnly);
}

void ADXCube::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		FTimerHandle TimerHandle01;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle01, FTimerDelegate::CreateLambda
		([&]() -> void
			{
				float RandomR = FMath::RandRange(0.f, 1.f);
				float RandomG = FMath::RandRange(0.f, 1.f);
				float RandomB = FMath::RandRange(0.f, 1.f);
				ServerLightColor = FLinearColor(RandomR, RandomG, RandomB, 1.f);
				OnRep_ServerLightColor();
			}
		), 1.f, true);

		FTimerHandle TimerHandle02;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle02, FTimerDelegate::CreateLambda
		([&]() ->void
			{
				FlushNetDormancy();
			}
		), 5.f, false);
	}
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

	DrawDebugSphere(GetWorld(), GetActorLocation(), NetCullDistance / 2.f, 16, FColor::Green, false, -1.f);
}

bool ADXCube::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
	bool bIsNetRelevant = Super::IsNetRelevantFor(RealViewer, ViewTarget, SrcLocation);

	if (!bIsNetRelevant)
	{
		DX_LOG_NET(LogDXNet, Log, TEXT("%s is not relevant for(%s %s)"), *GetName(), *RealViewer->GetName(), *ViewTarget->GetName());
	}
	return bIsNetRelevant;	
}

void ADXCube::OnRep_ServerRotationYaw()
{
	SetActorRotation(FRotator(0.f, ServerRotationYaw, 0.f));
	AccDeltaSecondSinceReplicated = 0.f;
}

void ADXCube::OnRep_ServerLightColor()
{
	if (HasAuthority())
	{
		DX_LOG_NET(LogDXNet, Log, TEXT("OnRep_ServerLightColor(): %s"), *ServerLightColor.ToString());
	}
	PointLight->SetLightColor(ServerLightColor);
}