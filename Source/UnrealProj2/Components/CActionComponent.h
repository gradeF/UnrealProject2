#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"


UENUM( BlueprintType )
enum class EActionType : uint8
{
	Unarmed,
	OneHand,
	TwoHand,
	Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FActionTypeChanged, EActionType, InPrevType, EActionType, InNewType );

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJ2_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCActionComponent();

	UFUNCTION( BlueprintPure )
		FORCEINLINE bool IsUnarmedMode() { return Type == EActionType::Unarmed; }
	UFUNCTION( BlueprintPure )
		FORCEINLINE bool IsOneHandMode() { return Type == EActionType::OneHand; }
	UFUNCTION( BlueprintPure )
		FORCEINLINE bool IsTwoHandMode() { return Type == EActionType::TwoHand; }
	UFUNCTION( BlueprintPure )
		FORCEINLINE class UCActionData* GetCurrent() { return Datas[(int32)Type]; }

	void SetUnarmedMode();
	void SetOneHandMode();
	void SetTwoHandMode();
	void DoAction();

protected:
	virtual void BeginPlay() override;

private:
	void ChangeType( EActionType InNewType );
	void SetMode( EActionType InType );

public:	
	UPROPERTY( BlueprintAssignable )
		FActionTypeChanged OnActionTypeChanged;

private:
	EActionType Type;

	UPROPERTY( EditDefaultsOnly, Category = "Weapons" )
		class UCActionData* Datas[(int32)EActionType::Max];
};

