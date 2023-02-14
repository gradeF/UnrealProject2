#include "CPlayer.h"

#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"

#include "Components/InputComponent.h"
#include "Components/COptionComponent.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"

#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

ACPlayer::ACPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USpringArmComponent>( this, &SpringArm, "SpringArm", GetMesh() );
	CHelpers::CreateComponent<UCameraComponent>( this, &Camera, "Camera", SpringArm );

	CHelpers::CreateActorComponent<UCMontagesComponent>( this, &Montages, "Montages" );
	CHelpers::CreateActorComponent<UCOptionComponent>( this, &Option, "Option");
	CHelpers::CreateActorComponent<UCStatusComponent>( this, &Status, "Status");
	CHelpers::CreateActorComponent<UCStateComponent>( this, &State, "State");
	CHelpers::CreateActorComponent<UCActionComponent>( this, &Action, "Action" );


	bUseControllerRotationYaw = false;

	GetMesh()->SetRelativeLocation( FVector( 0, 0, -90 ) );
	GetMesh()->SetRelativeRotation( FRotator( 0,-90,0 ) );

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>( &mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'" );
	GetMesh()->SetSkeletalMesh( mesh );

	//SpringArm->SetRelativeLocation( FVector( 0, 0, 140 ) );
	//SpringArm->SetRelativeRotation( FRotator( 0, 90, 0 ) );
	SpringArm->SetRelativeLocationAndRotation( { 0,0,140 }, { 0,90,0 } );
	SpringArm->TargetArmLength = 300.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	GetCharacterMovement()->RotationRate = FRotator( 0, 720, 0 );
	GetCharacterMovement()->bOrientRotationToMovement = true;

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>( &animInstance, "AnimBlueprint'/Game/Player/ABP_CPlayer.ABP_CPlayer_C'" );
	GetMesh()->SetAnimInstanceClass( animInstance );
	
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	State->OnStateTypeChanged.AddDynamic( this, &ACPlayer::OnStateTypeChanged );

	UMaterialInstanceConstant* body;
	UMaterialInstanceConstant* logo;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>( &body, "MaterialInstanceConstant'/Game/Materials/M_UE4Man_Body_Inst.M_UE4Man_Body_Inst'" );
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>( &logo, "MaterialInstanceConstant'/Game/Materials/M_UE4Man_ChestLogo_Inst.M_UE4Man_ChestLogo_Inst'" );

	BodyMaterial = UMaterialInstanceDynamic::Create( body, this );
	LogoMaterial = UMaterialInstanceDynamic::Create( logo, this );

	GetMesh()->SetMaterial( 0, BodyMaterial );
	GetMesh()->SetMaterial( 1, LogoMaterial );
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis( "MoveForward", this, &ACPlayer::OnMoveForward );
	PlayerInputComponent->BindAxis( "MoveRight", this, &ACPlayer::OnMoveRight );
	PlayerInputComponent->BindAxis( "HorizontalLook", this, &ACPlayer::OnHorizontalLook );
	PlayerInputComponent->BindAxis( "VerticalLook", this, &ACPlayer::OnVerticalLook );
	PlayerInputComponent->BindAction( "Avoid", EInputEvent::IE_Pressed, this, &ACPlayer::OnAvoid );
	PlayerInputComponent->BindAction( "OneHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnOneHand );
	PlayerInputComponent->BindAction("OneHand", EInputEvent::IE_Pressed, this, &ACPlayer::OnTwoHand);
	PlayerInputComponent->BindAction( "Action", EInputEvent::IE_Pressed, this, &ACPlayer::OnDoAction );
}

void ACPlayer::OnMoveForward( float InAxis )
{
	CheckFalse( Status->CanMove() );
	FRotator rotator = FRotator( 0, GetControlRotation().Yaw, 0 );
	FVector direction = FQuat( rotator ).GetForwardVector();
	AddMovementInput( direction, InAxis );
}

void ACPlayer::OnMoveRight( float InAxis )
{
	CheckFalse( Status->CanMove() );
	FRotator rotator = FRotator( 0, GetControlRotation().Yaw, 0 );
	FVector direction = FQuat( rotator ).GetRightVector();
	AddMovementInput( direction, InAxis );
}

void ACPlayer::OnHorizontalLook( float InAxis )
{
	float rate = Option->GetHorizontalLookRate();
	AddControllerYawInput( InAxis * rate * GetWorld()->GetDeltaSeconds() );
}

void ACPlayer::OnVerticalLook( float InAxis )
{
	float rate = Option->GetVerticalLookRate();

	float inputVal = Option->bInvertVerticalLook ? -InAxis : InAxis;

	AddControllerPitchInput( inputVal * rate * GetWorld()->GetDeltaSeconds() );
}

void ACPlayer::OnAvoid()
{
	CheckFalse( Status->CanMove() );
	CheckFalse( State->IsIdleMode() );

	if ( InputComponent->GetAxisValue( "MoveForward" ) < 0.0f )
	{
		State->SetBackstepMode();
		return;
	}
	State->SetRollMode();
}

void ACPlayer::OnOneHand()
{
	CheckFalse( State->IsIdleMode() );
	Action->SetOneHandMode();
}
void ACPlayer::OnTwoHand()
{
	CheckFalse(State->IsIdleMode());
	Action->SetTwoHandMode();
}
void ACPlayer::OnDoAction()
{
	Action->DoAction();
}

void ACPlayer::Begin_Roll()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	FVector start = GetActorLocation();
	FVector target = start + GetVelocity().GetSafeNormal2D();
	SetActorRotation( UKismetMathLibrary::FindLookAtRotation( start, target ) );
	Montages->PlayRoll();

}

void ACPlayer::Begin_Backstep()
{
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	Montages->PlayBackstep();

}

void ACPlayer::End_Roll()
{
	if ( Action->IsUnarmedMode() == false )
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	State->SetIdleMode();
}

void ACPlayer::End_Backstep()
{

	if ( Action->IsUnarmedMode() )
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

	State->SetIdleMode();

}

void ACPlayer::OnStateTypeChanged( EStateType InPrevType, EStateType InNewtype )
{
	switch ( InNewtype )
	{
	case EStateType::Idle:
		break;
	case EStateType::Roll:
		Begin_Roll();
		break;
	case EStateType::Backstep:
		Begin_Backstep();
		break;
	}
}

void ACPlayer::ChangeColor( FLinearColor InColor )
{
	BodyMaterial->SetVectorParameterValue( "BodyColor", InColor );
	LogoMaterial->SetVectorParameterValue( "BodyColor", InColor );
}
