#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actions/CActionData.h"
#include "Actions/CAttachment.h"
#include "CEquipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FEquipmentDelegate );
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FUnequipmentDelegate );

UCLASS()
class UNREALPROJ2_API ACEquipment : public AActor
{
	GENERATED_BODY()
	
public:	
	ACEquipment();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION( BlueprintNativeEvent )
		void Equip();
	void Equip_Implementation();

	UFUNCTION( BlueprintNativeEvent )
		void Begin_Equip();
	void Begin_Equip_Implementation();

	UFUNCTION( BlueprintNativeEvent )
		void End_Equip();
	void End_Equip_Implementation();

	UFUNCTION( BlueprintNativeEvent )
		void Unequip();
	void Unequip_Implementation();
	
	FORCEINLINE void SetData( FEquipmentData InData ) { Data = InData; }
	FORCEINLINE void SetColor( FLinearColor InColor ) { Color = InColor; }

public:
	UPROPERTY( BlueprintAssignable )
		FEquipmentDelegate OnEquipmentDelegate;
	UPROPERTY( BlueprintAssignable )
		FUnequipmentDelegate OnUnequipmentDelegate;

protected:
	UPROPERTY( BlueprintReadOnly )
		class ACharacter* OwnerCharacter;
	UPROPERTY( BlueprintReadOnly )
		class UCStateComponent* State;
	UPROPERTY( BlueprintReadOnly )
		class UCStatusComponent* Status;
private:
	FEquipmentData Data;
	FLinearColor Color;

};
