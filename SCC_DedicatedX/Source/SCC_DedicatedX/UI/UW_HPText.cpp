#include "UI/UW_HPText.h"

#include "Character/DXPlayerCharacter.h"
#include "Component/DXStatusComponent.h"
#include "Components/TextBlock.h"

UUW_HPText::UUW_HPText(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

void UUW_HPText::NativeConstruct()
{
	Super::NativeConstruct();

	ADXPlayerCharacter* OwningCharacter = Cast<ADXPlayerCharacter>(OwningActor);
	if (IsValid(OwningActor))
	{
		OwningCharacter->SetHPTextWidget(this);
	}
}

void UUW_HPText::InitializeHPTextWidget(UDXStatusComponent* InStatusComponent)
{
	OnMaxHPChange(InStatusComponent->GetMaxHP());
	OnCurrentHPChange(InStatusComponent->GetCurrentHP());
}

void UUW_HPText::OnMaxHPChange(float InMaxHP)
{	
	MaxHPText->SetText(FText::FromString(FString::SanitizeFloat(InMaxHP)));
}

void UUW_HPText::OnCurrentHPChange(float InCurrentHP)
{
	CurrentHPText->SetText(FText::FromString(FString::SanitizeFloat(InCurrentHP)));
}
