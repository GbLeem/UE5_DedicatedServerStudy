#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DXHPTextWidgetComponent.generated.h"


UCLASS()
class SCC_DEDICATEDX_API UDXHPTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	virtual void InitWidget() override;

};
