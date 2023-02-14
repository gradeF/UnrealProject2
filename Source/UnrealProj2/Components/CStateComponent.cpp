#include "CStateComponent.h"

UCStateComponent::UCStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UCStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCStateComponent::SetIdleMode()
{
	ChangeType( EStateType::Idle );
}

void UCStateComponent::SetRollMode()
{
	ChangeType( EStateType::Roll );
}

void UCStateComponent::SetBackstepMode()
{
	ChangeType( EStateType::Backstep );
}

void UCStateComponent::SetEquipMode()
{
	ChangeType( EStateType::Equip );
}

void UCStateComponent::SetUnequipMode()
{
	ChangeType( EStateType::Unequip );
}

void UCStateComponent::SetActionMode()
{
	ChangeType( EStateType::Action );
}

void UCStateComponent::SetHittedMode()
{
	ChangeType( EStateType::Hitted );
}

void UCStateComponent::ChangeType( EStateType InType )
{
	EStateType type = Type;
	Type = InType;
	if ( OnStateTypeChanged.IsBound() )
	{
		OnStateTypeChanged.Broadcast(type, InType);
	}
}

