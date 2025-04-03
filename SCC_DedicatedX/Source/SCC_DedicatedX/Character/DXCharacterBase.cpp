#include "Character/DXCharacterBase.h"

#include "SCC_DedicatedX.h"
ADXCharacterBase::ADXCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADXCharacterBase::PossessedBy(AController* NewController)
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	AActor* OwnerActor = GetOwner();
	if (IsValid(OwnerActor) == true)
	{
		DX_LOG_NET(LogDXNet, Log, TEXT("OwnerActor Name: %s"), *OwnerActor->GetName());
	}
	else
	{
		DX_LOG_NET(LogDXNet, Log, TEXT("There is no OwnerActor."));
	}

	Super::PossessedBy(NewController);

	if (IsValid(OwnerActor) == true)
	{
		DX_LOG_NET(LogDXNet, Log, TEXT("OwnerActor Name: %s"), *OwnerActor->GetName());
	}
	else
	{
		DX_LOG_NET(LogDXNet, Log, TEXT("There is no OwnerActor."));
	}

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));
}

void ADXCharacterBase::OnRep_Owner()
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	Super::OnRep_Owner();

	AActor* OwnerActor = GetOwner();
	if (IsValid(OwnerActor) == true)
	{
		DX_LOG_NET(LogDXNet, Log, TEXT("OwnerActor Name: %s"), *OwnerActor->GetName());
	}
	else
	{
		DX_LOG_NET(LogDXNet, Log, TEXT("There is no OwnerActor."));
	}

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));
}

void ADXCharacterBase::PostNetInit()
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	Super::PostNetInit();

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));
}