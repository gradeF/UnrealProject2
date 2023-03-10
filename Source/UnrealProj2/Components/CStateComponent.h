#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM( BlueprintType )
enum class EStateType : uint8
{
	Idle,
	Roll,
	Backstep,
	Equip,
	Unequip,
	Action,
	Hitted,
	Max
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType );

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJ2_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	UFUNCTION( BlueprintPure )
		FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
	UFUNCTION( BlueprintPure )
		FORCEINLINE bool IsRollMode() { return Type == EStateType::Roll; }
	UFUNCTION( BlueprintPure )
		FORCEINLINE bool IsBackStepMode() { return Type == EStateType::Backstep; }
	UFUNCTION( BlueprintPure )
		FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }
	UFUNCTION( BlueprintPure )
		FORCEINLINE bool IsUnequipMode() { return Type == EStateType::Unequip; }
	UFUNCTION( BlueprintPure )
		FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
	UFUNCTION( BlueprintPure )
		FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }

	void SetIdleMode();
	void SetRollMode();
	void SetBackstepMode();
	void SetEquipMode();
	void SetUnequipMode();
	void SetActionMode();
	void SetHittedMode();

private:
	void ChangeType( EStateType InType );

public:
	UPROPERTY( BlueprintAssignable )
		FStateTypeChanged OnStateTypeChanged;

private:
	EStateType Type;
		
};
