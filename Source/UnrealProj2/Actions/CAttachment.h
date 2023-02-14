// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams( FAttachmentBeginOverlap,
class ACharacter*, InAttacker,
class AActor*, InAttackCauser,
	class ACharacter*, InOtherCharacter );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams( FAttachmentEndOverlap,
class ACharacter*, InAttacker,
	class AActor*, InAttackCauser,
	class ACharacter*, InOtherCharacter );


UCLASS()
class UNREALPROJ2_API ACAttachment : public AActor
{
	GENERATED_BODY()
	
public:	
	ACAttachment();
	UFUNCTION( BlueprintImplementableEvent )
		void OnEquip();
	UFUNCTION( BlueprintImplementableEvent )
		void OnUnequip();

	void OnCollision();
	void OffCollision();

protected:
	virtual void BeginPlay() override;

	UFUNCTION( BlueprintCallable )
		void AttachTo( FName InSocketName );
	
private:
	UFUNCTION()
		void OnComponentBeginOverlap(
			 UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult
		);

	UFUNCTION()
		void OnComponentEndOverlap(
			UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex
		);


public:	
	UPROPERTY( BlueprintReadOnly )
		class ACharacter* OwnerCharacter;
	UPROPERTY( BlueprintReadOnly )
		class UCStateComponent* State;
	UPROPERTY( BlueprintReadOnly )
		class UCStatusComponent* Status;

	UPROPERTY( BlueprintAssignable )
		FAttachmentBeginOverlap OnAttachmentBeginOverlap;
	UPROPERTY( BlueprintAssignable )
		FAttachmentEndOverlap OnAttachmentEndOverlap;

protected:
	UPROPERTY( BlueprintReadOnly, VisibleDefaultsOnly )
		class USceneComponent* Scene;

private:
	TArray<class UShapeComponent*> ShapeComponents;

};
