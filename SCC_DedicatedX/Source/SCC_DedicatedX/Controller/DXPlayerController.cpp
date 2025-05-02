#include "Controller/DXPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"

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
		}
	}
}

void ADXPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}
