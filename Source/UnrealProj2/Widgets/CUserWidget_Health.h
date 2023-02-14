#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Health.generated.h"

UCLASS()
class UNREALPROJ2_API UCUserWidget_Health : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION( BlueprintImplementableEvent )
		void UpdateHealth( float Health, float Maxhealth );
};
