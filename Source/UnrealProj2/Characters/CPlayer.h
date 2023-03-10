#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Characters/ICharacter.h"
#include "CPlayer.generated.h"

UCLASS()
class UNREALPROJ2_API ACPlayer : public ACharacter, public IICharacter
{
	GENERATED_BODY()

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnMoveForward( float InAxis );
	void OnMoveRight( float InAxis );
	void OnHorizontalLook( float InAxis );
	void OnVerticalLook( float InAxis );

	void OnAvoid();
	void OnOneHand();
	void OnTwoHand();
	void OnDoAction();

private:
	void Begin_Roll();
	void Begin_Backstep();
public:
	void End_Roll();
	void End_Backstep();

private:
	UFUNCTION()
		void OnStateTypeChanged( EStateType InPrevType, EStateType InNewtype );

private:
	UPROPERTY( VisibleDefaultsOnly )
		class USpringArmComponent* SpringArm;
	UPROPERTY( VisibleDefaultsOnly )
		class UCameraComponent* Camera;

	UPROPERTY( VisibleDefaultsOnly )
		class UCOptionComponent* Option;

	UPROPERTY( VisibleDefaultsOnly )
		class UCStatusComponent* Status;
	UPROPERTY( VisibleDefaultsOnly )
		class UCStateComponent* State;
	UPROPERTY( VisibleDefaultsOnly )
		class UCActionComponent* Action;

	UPROPERTY( VisibleDefaultsOnly )
		class UCMontagesComponent* Montages;

	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* LogoMaterial;

public:
	virtual void ChangeColor( FLinearColor InColor ) override;
};
