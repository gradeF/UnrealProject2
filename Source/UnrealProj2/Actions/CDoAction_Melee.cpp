#include "CDoAction_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CStatuscomponent.h"

void ACDoAction_Melee::DoAction()
{
	Super::DoAction();
	CheckFalse( Datas.Num() > 0 );

	// 다음 콤보가 허용 되면
	if ( bEnable == true )
	{
		bExist = true;
		bEnable = false;
		return;
	}

	CheckFalse( State->IsIdleMode() );
	State->SetActionMode();

	const FDoActionData& data = Datas[0];
	OwnerCharacter->PlayAnimMontage( data.AnimMontage, data.PlayRatio, data.StartSection );

	data.bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();

	CheckFalse( bExist );
	bExist = false;

	OwnerCharacter->StopAnimMontage();
	++Index;
	const FDoActionData& data = Datas[Index];
	OwnerCharacter->PlayAnimMontage( data.AnimMontage, data.PlayRatio, data.StartSection );
	data.bCanMove ? Status->SetMove() : Status->SetStop();
}

void ACDoAction_Melee::End_DoAction()
{
	Super::End_DoAction();

	const FDoActionData& data = Datas[Index];
	OwnerCharacter->StopAnimMontage( data.AnimMontage );

	Index = 0;

	State->SetIdleMode();
	Status->SetMove();
}

void ACDoAction_Melee::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOtherCharacter);

	CheckNull(InOtherCharacter);

	UParticleSystem* hitEffect = Datas[Index].Effect;
	if (!!hitEffect)
	{
		FTransform transform = Datas[Index].EffectTransform;
		transform.AddToTranslation(InOtherCharacter->GetActorLocation());

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), hitEffect, transform);
	}

	float hitStop = Datas[Index].HitStop;
	if (FMath::IsNearlyZero(hitStop) == false)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1e-1f);
		UKismetSystemLibrary::K2_SetTimer(this, "RestoreDilation", hitStop * 1e-1f, false);
	}

	TSubclassOf<UCameraShake> shake = Datas[Index].ShakeClass;
	if (shake != NULL)
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->PlayerCameraManager->PlayCameraShake(shake);
	}
	FDamageEvent e;
	InOtherCharacter->TakeDamage( Datas[Index].Power, e, OwnerCharacter->GetController(), this );

}

void ACDoAction_Melee::OnAttachmentEndOverlap( ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOtherCharacter )
{
}

void ACDoAction_Melee::RestoreDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}
