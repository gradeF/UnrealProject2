#include "CActionComponent.h"
#include "Global.h"
#include "Actions/CActionData.h"
#include "GameFramework/Character.h"
#include "Actions/CDoAction.h"
#include "Actions/CEquipment.h"

UCActionComponent::UCActionComponent()
{
}

void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ACharacter* character = Cast<ACharacter>( GetOwner() );
	for ( int32 i = 0; i < (int32)EActionType::Max; ++i )
	{
		if ( !!Datas[i] )
			Datas[i]->BeginPlay( character );
	}
}

void UCActionComponent::ChangeType( EActionType InNewType )
{
	EActionType prevType = Type;
	Type = InNewType;

	if ( OnActionTypeChanged.IsBound() )
	{
		OnActionTypeChanged.Broadcast( prevType, InNewType );
	}
}

void UCActionComponent::SetMode( EActionType InType )
{
	if ( Type == InType )
	{
		SetUnarmedMode();
		return;
	}
	else if ( IsUnarmedMode() == false )
	{
		ACEquipment* equipment = Datas[(int32)Type]->GetEquipment();
		CheckNull( equipment );

		equipment->Unequip();
	}

	if ( !!Datas[(int32)InType] )
	{
		ACEquipment* equipment = Datas[(int32)InType]->GetEquipment();
		CheckNull( equipment );

		equipment->Equip();
	}

	ChangeType( InType );
}


void UCActionComponent::SetUnarmedMode()
{
	//SetMode( EActionType::Unarmed );

	if ( !!Datas[(int32)Type] )
	{
		ACEquipment* equipment = Datas[(int32)Type]->GetEquipment();
		CheckNull( equipment );

		equipment->Unequip();
	}

	ACEquipment* equipment = Datas[(int32)Type]->GetEquipment();
	CheckNull( equipment );
	equipment->Equip();

	ChangeType( EActionType::Unarmed );
}

void UCActionComponent::SetOneHandMode()
{
	SetMode( EActionType::OneHand );
}

void UCActionComponent::SetTwoHandMode()
{
	SetMode( EActionType::TwoHand );
}

void UCActionComponent::DoAction()
{
	CheckTrue( IsUnarmedMode() );
	if ( !!Datas[(int32)Type] )
	{
		ACDoAction* action = Datas[(int32)Type]->GetDoAction();

		if ( !!action )
			action->DoAction();
	}
}
