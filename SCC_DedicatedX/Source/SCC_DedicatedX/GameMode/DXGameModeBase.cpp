#include "GameMode/DXGameModeBase.h"

#include "Controller/DXPlayerController.h"
#include "GameState/DXGameStateBase.h"

void ADXGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	//player access prevention -> not a good approach
	ADXGameStateBase* DXGameState = GetGameState<ADXGameStateBase>();
	if (!IsValid(DXGameState))
	{
		return;
	}
	if (DXGameState->MatchState != EMatchState::Waiting)
	{
		NewPlayer->SetLifeSpan(0.1f);
		return;
	}


	ADXPlayerController* NewPlayerController = Cast<ADXPlayerController>(NewPlayer);
	if (IsValid(NewPlayerController))
	{
		AlivePlayerControllers.Add(NewPlayerController);

		NewPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server"));
	}	
}

void ADXGameModeBase::Logout(AController* Existing)
{
	Super::Logout(Existing);

	ADXPlayerController* ExistingPlayerController = Cast<ADXPlayerController>(Existing);
	if (IsValid(ExistingPlayerController) && AlivePlayerControllers.Find(ExistingPlayerController) != INDEX_NONE)
	{
		AlivePlayerControllers.Remove(ExistingPlayerController);
		DeadPlayerControllers.Add(ExistingPlayerController);
	}
}

void ADXGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(MainTimerHandle, this, &ThisClass::OnMainTimerElapsed, 1.f, true);

	RemainWaitingTimeForPlaying = WaitingTime;
}

void ADXGameModeBase::OnMainTimerElapsed()
{
	ADXGameStateBase* DXGameState = GetGameState<ADXGameStateBase>();

	if (!IsValid(DXGameState))
	{
		return;
	}

	switch (DXGameState->MatchState)
	{
	case EMatchState::None:
		break;
	case EMatchState::Waiting:
	{
		FString NotificationString = FString::Printf(TEXT(""));
		if (AlivePlayerControllers.Num() < MinimumPlayerCountForPlaying)
		{
			NotificationString = FString::Printf(TEXT("Wait another players for playing"));

			RemainWaitingTimeForPlaying = WaitingTime;
		}
		else
		{
			NotificationString = FString::Printf(TEXT("Wait %d seconds for playing"), RemainWaitingTimeForPlaying);

			--RemainWaitingTimeForPlaying;
		}
		if (RemainWaitingTimeForPlaying <= 0)
		{
			NotificationString = FString::Printf(TEXT(""));
			DXGameState->MatchState = EMatchState::Playing;
		}
		NotifyToAllPlayer(NotificationString);
		break;
	}	
	case EMatchState::Playing:
		break;
	case EMatchState::Enging:
		break;
	case EMatchState::End:
		break;
	default:
		break;
	}
}

void ADXGameModeBase::NotifyToAllPlayer(const FString& NotificationString)
{
	for (auto AlivePlayerController : AlivePlayerControllers)
	{
		AlivePlayerController->NotificationText = FText::FromString(NotificationString);
	}

	for (auto DeadPlayerController : DeadPlayerControllers)
	{
		DeadPlayerController->NotificationText = FText::FromString(NotificationString);
	}
}
