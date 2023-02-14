#include "CEquipment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatuscomponent.h"
#include "Characters/ICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

ACEquipment::ACEquipment()
{
}

void ACEquipment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>( GetOwner() );
	State = CHelpers::GetComponent<UCStateComponent>( OwnerCharacter );
	Status = CHelpers::GetComponent<UCStatusComponent>( OwnerCharacter );

	// Super::BeginPlay에서 Blueprint 내 함수를 시작하기 때문에 이전에 초기화 해야 BP에서 사용 가능
	Super::BeginPlay();
	
}

void ACEquipment::Equip_Implementation()
{
	State->SetEquipMode();
	
	if ( Data.AnimMontage != NULL )
	{
		OwnerCharacter->PlayAnimMontage( Data.AnimMontage, Data.PlayRatio, Data.StartSection );
	}
	else
	{
		End_Equip();
	}


	if ( Data.bPawnControl == true )
	{
		OwnerCharacter->bUseControllerRotationYaw = true;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	IICharacter* character = Cast<IICharacter>( OwnerCharacter );
	CheckNull( character );
	character->ChangeColor( Color );
}

void ACEquipment::Begin_Equip_Implementation()
{
	if ( OnEquipmentDelegate.IsBound() )
	{
		OnEquipmentDelegate.Broadcast();
	}
}

void ACEquipment::End_Equip_Implementation()
{
	State->SetIdleMode();
}

void ACEquipment::Unequip_Implementation()
{
	if ( OnUnequipmentDelegate.IsBound() )
	{
		OnUnequipmentDelegate.Broadcast();
	}

	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}
