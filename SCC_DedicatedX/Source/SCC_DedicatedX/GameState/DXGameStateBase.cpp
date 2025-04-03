#include "GameState/DXGameStateBase.h"

#include "SCC_DedicatedX.h"

ADXGameStateBase::ADXGameStateBase()
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));
}

void ADXGameStateBase::HandleBeginPlay()
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	//서버 로직, 월드의 액터들에게 BeginPlay() 호출하도록 함
	//호출 후 OnRep_ReplicatedHasBegunPlay() 호출된다.
	Super::HandleBeginPlay(); 

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));
}

void ADXGameStateBase::OnRep_ReplicatedHasBegunPlay()
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	Super::OnRep_ReplicatedHasBegunPlay();

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));
}
