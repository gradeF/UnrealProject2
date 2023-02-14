#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "COptionComponent.generated.h"


UCLASS( ClassGroup=(GameProject), meta=(BlueprintSpawnableComponent) )
class UNREALPROJ2_API UCOptionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCOptionComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE float GetHorizontalLookRate() { return HorizontalLookRate; }
	FORCEINLINE float GetVerticalLookRate() { return VerticalLookRate; }

public:
	UPROPERTY( EditDefaultsOnly )
		bool bInvertVerticalLook = true;

private:
	UPROPERTY( EditDefaultsOnly )
		float HorizontalLookRate = 45;
	UPROPERTY( EditDefaultsOnly )
		float VerticalLookRate = 45;


};
