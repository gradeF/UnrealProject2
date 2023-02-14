#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/ICharacter.h"
#include "Components/CStateComponent.h"
#include "CEnemy.generated.h"

UCLASS()
class UNREALPROJ2_API ACEnemy : public ACharacter, public IICharacter
{
	GENERATED_BODY()

public:
	ACEnemy();
	virtual void Tick( float DeltaTime ) override;
	virtual void ChangeColor( FLinearColor InColor ) override;
	float TakeDamage( float Damage, struct FDamageEvent const& DamgeEvent, AController* EventInstigator, AActor* DamageCauser ) override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnStateTypeChanged( EStateType InPrevType, EStateType InNewType );
	UFUNCTION()
		void RestoreColor();
	void Hitted();
	
protected:
	UPROPERTY( VisibleDefaultsOnly )
		class UCStatusComponent* Status;
	UPROPERTY( VisibleDefaultsOnly )
		class UCStateComponent* State;
	UPROPERTY( VisibleDefaultsOnly )
		class UCActionComponent* Action;
	UPROPERTY( VisibleDefaultsOnly )
		class UCMontagesComponent* Montages;

	UPROPERTY( VisibleDefaultsOnly )
		class UWidgetComponent* NameWidget;
	UPROPERTY( VisibleDefaultsOnly )
		class UWidgetComponent* HealthWidget;

private:
	class UMaterialInstanceDynamic* BodyMaterial;
	class UMaterialInstanceDynamic* LogoMaterial;

	class AController* DamageInstigator;
	float DamageValue;

	UPROPERTY( EditAnywhere, Category = "Hitted" )
		float LaunchAmount = 100.0f;

};
