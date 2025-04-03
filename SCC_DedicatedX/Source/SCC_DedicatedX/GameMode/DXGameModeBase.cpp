#include "GameMode/DXGameModeBase.h"

#include "SCC_DedicatedX.h"

ADXGameModeBase::ADXGameModeBase()
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));
}

void ADXGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	//ErrorMessage = TEXT("Server is full. try again");

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));
}

APlayerController* ADXGameModeBase::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	APlayerController* LoginPC = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));

	return LoginPC;
}

void ADXGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	Super::PostLogin(NewPlayer);

	UNetDriver* NetDriver = GetNetDriver();
	if (IsValid(NetDriver))
	{
		if (NetDriver->ClientConnections.Num() == 0)
		{
			DX_LOG_NET(LogDXNet, Log, TEXT("There is no ClientConnection"));
		}
		else
		{
			for (const auto& ClientConnection : NetDriver->ClientConnections)
			{
				if (IsValid(ClientConnection))
				{
					DX_LOG_NET(LogDXNet, Log, TEXT("ClientConnection Name: %s"), *ClientConnection->GetName());
				}
			}
		}
	}
	else
	{
		DX_LOG_NET(LogDXNet, Log, TEXT("There is no NetDriver"));
	}

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));
}

void ADXGameModeBase::StartPlay()
{
	DX_LOG_NET(LogDXNet, Log, TEXT("Begin"));

	Super::StartPlay();

	DX_LOG_NET(LogDXNet, Log, TEXT("End"));
}
