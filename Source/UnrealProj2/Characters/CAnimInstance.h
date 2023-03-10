#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CActionComponent.h"
#include "CAnimInstance.generated.h"

UCLASS()
class UNREALPROJ2_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation( float DeltaSeconds ) override;

private:
	UFUNCTION()
		void OnActionTypeChanged( EActionType InPrevType, EActionType InNewType );

protected:
	UPROPERTY( BlueprintReadOnly, EditAnywhere )
		float Speed;
	UPROPERTY( BlueprintReadOnly, EditAnywhere )
		float Direction;
	UPROPERTY( BlueprintReadOnly, EditAnywhere )
		EActionType ActionType;

private:
	ACharacter* character;
};
