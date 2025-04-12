#include "Component/DXHPTextWidgetComponent.h"

#include "UI/UW_HPText.h"

void UDXHPTextWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UUW_HPText* HPText = Cast<UUW_HPText>(GetWidget());
	if (IsValid(HPText))
	{
		HPText->SetOwningActor(GetOwner());
	}
}