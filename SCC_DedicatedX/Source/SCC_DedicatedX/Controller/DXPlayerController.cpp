#include "Controller/DXPlayerController.h"

#include "GameMode/DXGameModeBase.h"
#include "UI/UW_GameResult.h"

#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

void ADXPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//server check
	if (!IsLocalController())
	{
		return;
	}

	if (IsValid(NotificationTextUIClass))
	{
		UUserWidget* NotificationTextUI = CreateWidget<UUserWidget>(this, NotificationTextUIClass);
		if (IsValid(NotificationTextUI))
		{
			NotificationTextUI->AddToViewport(1);
			NotificationTextUI->SetVisibility(ESlateVisibility::Visible);

			FInputModeGameOnly Mode;			
			SetInputMode(Mode);

			bShowMouseCursor = false;
		}
	}
}

void ADXPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}

void ADXPlayerController::OnCharaterDead()
{
	ADXGameModeBase* GameMode = Cast<ADXGameModeBase>(UGameplayStatics::GetGameMode(this));
	//server
	if (HasAuthority() && IsValid(GameMode))
	{
		GameMode->OnCharacterDead(this);
	}
}

void ADXPlayerController::ClientRPCReturnToTitle_Implementation()
{
	//only client's level change
	if (IsLocalController())
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("Title")), true);
	}
}

void ADXPlayerController::ClientRPCShowGameResultWidget_Implementation(int32 InRanking)
{
	if (IsLocalController())
	{
		if (IsValid(GameResultUIClass))
		{
			UUW_GameResult* GameResultUI = CreateWidget<UUW_GameResult>(this, GameResultUIClass);
			if (IsValid(GameResultUI))
			{
				GameResultUI->AddToViewport(3);

				FString GameResultString = FString::Printf(TEXT("%s"), InRanking == 1 ? TEXT("Winner Winner Chicken Dinner!") : TEXT("Better Luck Next Time!"));
				GameResultUI->ResultText->SetText(FText::FromString(GameResultString));

				FString RankingString = FString::Printf(TEXT("#%02d"), InRanking);
				GameResultUI->RankingText->SetText(FText::FromString(RankingString));

				FInputModeUIOnly Mode;
				Mode.SetWidgetToFocus(GameResultUI->GetCachedWidget());
				SetInputMode(Mode);

				bShowMouseCursor = true;
			}
		
		}
	}
}
