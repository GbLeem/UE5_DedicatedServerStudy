#include "Gimmick/DXBuffBox.h"

#include "Character/DXPlayerCharacter.h"

#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
ADXBuffBox::ADXBuffBox()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
	TriggerBox->SetBoxExtent(FVector(40.f, 42.f, 30.f));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(TriggerBox);
	Mesh->SetRelativeLocation(FVector(0.f, -3.5f, -30.f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));

	ParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleEffect"));
	ParticleEffect->SetupAttachment(TriggerBox);
	ParticleEffect->bAutoActivate = false;
}

void ADXBuffBox::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);
}

void ADXBuffBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	ParticleEffect->Activate(true);
	Mesh->SetHiddenInGame(true);
	SetActorEnableCollision(false);
	ParticleEffect->OnSystemFinished.AddDynamic(this, &ThisClass::OnEffectFinished);

	ADXPlayerCharacter* OverlappingCharacter = Cast<ADXPlayerCharacter>(OtherActor);
	if (IsValid(OverlappingCharacter))
	{
		OverlappingCharacter->TakeBuff(50.f);
	}
}

void ADXBuffBox::OnEffectFinished(UParticleSystemComponent* ParticleSystem)
{
	if (HasAuthority())
	{
		Destroy();
	}
}
