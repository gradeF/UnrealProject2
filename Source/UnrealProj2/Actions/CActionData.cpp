#include "CActionData.h"
#include "Global.h"
#include "CEquipment.h"
#include "CAttachment.h"
#include "GameFramework/Character.h"
#include "CDoAction.h"
#include "Components/SkeletalMeshComponent.h"

void UCActionData::BeginPlay( ACharacter* InOwnerCharacter )
{
	FTransform transform;

	if ( !!AttachmentClass )
	{
		Attachment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACAttachment>
			( AttachmentClass, transform, InOwnerCharacter );
		Attachment->SetActorLabel( InOwnerCharacter->GetActorLabel() + "_Attachment" );
		UGameplayStatics::FinishSpawningActor( Attachment, transform );
	}

	if ( !!EquipmentClass )
	{
		Equipment = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACEquipment>( EquipmentClass, transform, InOwnerCharacter );
		Equipment->AttachToComponent( InOwnerCharacter->GetMesh(), FAttachmentTransformRules( EAttachmentRule::KeepRelative, true ) );
		Equipment->SetActorLabel( InOwnerCharacter->GetActorLabel() + "_Equipment" );
		Equipment->SetData( EquipmentData );
		Equipment->SetColor( EquipmentColor );
		UGameplayStatics::FinishSpawningActor( Equipment, transform );
		if ( !!AttachmentClass )
		{
			Equipment->OnEquipmentDelegate.AddDynamic( Attachment, &ACAttachment::OnEquip );
			Equipment->OnUnequipmentDelegate.AddDynamic( Attachment, &ACAttachment::OnUnequip );
		}
	}

	if ( !!DoActionClass )
	{
		DoAction = InOwnerCharacter->GetWorld()->SpawnActorDeferred<ACDoAction>( DoActionClass, transform, InOwnerCharacter );
		DoAction->AttachToComponent( InOwnerCharacter->GetMesh(), FAttachmentTransformRules( EAttachmentRule::KeepRelative, true ) );
		DoAction->SetDatas( DoActionDatas );
		UGameplayStatics::FinishSpawningActor( DoAction, transform );

		if ( !!Attachment )
		{
			Attachment->OnAttachmentBeginOverlap.AddDynamic( DoAction, &ACDoAction::OnAttachmentBeginOverlap );
			Attachment->OnAttachmentEndOverlap.AddDynamic( DoAction, &ACDoAction::OnAttachmentEndOverlap );
		}
	}
}