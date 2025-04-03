#include "Controller/DXPlayerController.h"

#include "SCC_DedicatedX.h"

ADXPlayerController::ADXPlayerController()
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));
}

void ADXPlayerController::PostInitializeComponents()
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	Super::PostInitializeComponents();

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));
}

void ADXPlayerController::PostNetInit()
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	Super::PostNetInit();

	if (IsLocalPlayerController())
	{
		UNetDriver* NetDriver = GetNetDriver();
		if (IsValid(NetDriver))
		{
			UNetConnection* ServerConnection = NetDriver->ServerConnection;
			if (IsValid(ServerConnection))
			{
				DX_LOG_NET(LogDXNet, Log, TEXT("ServerConnection Name : %s"), * ServerConnection->GetName());
			}
			else
			{
				DX_LOG_NET(LogDXNet, Log, TEXT("There is no serverconnection"));
			}
		}
	}
	DX_LOG_NET(LogDXNet, Log, TEXT("End"));
}

void ADXPlayerController::BeginPlay()
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	Super::BeginPlay();

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));

	FInputModeGameOnly Mode;
	SetInputMode(Mode);

	bShowMouseCursor = false;
}

void ADXPlayerController::OnPossess(APawn* InPawn)
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	Super::OnPossess(InPawn);

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));
}
