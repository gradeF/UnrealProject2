#include "CEnemy.h"
#include "Global.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/CStatusComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "Components/WidgetComponent.h"
#include "Widgets/CUserWidget_Name.h"
#include "Widgets/CUserWidget_Health.h"
#include "Actions/CActionData.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation( FVector( 0, 0, -90 ) );
	GetMesh()->SetRelativeRotation( FRotator( 0, -90, 0 ) );

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>( &mesh, "SkeletalMesh'/Game/Character/Mesh/SK_Mannequin.SK_Mannequin'" );
	GetMesh()->SetSkeletalMesh( mesh );

	TSubclassOf<UAnimInstance> animInstance;
	CHelpers::GetClass<UAnimInstance>( &animInstance, "AnimBlueprint'/Game/Enemies/ABP_CEnemy.ABP_CEnemy_C'" );
	GetMesh()->SetAnimInstanceClass( animInstance );

	GetCharacterMovement()->RotationRate = FRotator( 0, 720, 0 );

	CHelpers::CreateActorComponent<UCMontagesComponent>( this, &Montages, "Montages" );
	CHelpers::CreateActorComponent<UCStatusComponent>( this, &Status, "Status" );
	CHelpers::CreateActorComponent<UCStateComponent>( this, &State, "State" );
	CHelpers::CreateActorComponent<UCActionComponent>( this, &Action, "Action" );

	CHelpers::CreateComponent<UWidgetComponent>( this, &NameWidget, "NameWidget", GetMesh() );
	CHelpers::CreateComponent<UWidgetComponent>( this, &HealthWidget, "HealthWidget", GetMesh() );

	TSubclassOf<UCUserWidget_Name> nameClass;
	CHelpers::GetClass<UCUserWidget_Name>( &nameClass, "WidgetBlueprint'/Game/Widgets/WB_Name.WB_Name_C'" );
	NameWidget->SetWidgetClass( nameClass );
	NameWidget->SetRelativeLocation( FVector( 0, 0, 240 ) );
	NameWidget->SetDrawSize( FVector2D( 240, 30 ) );
	NameWidget->SetWidgetSpace( EWidgetSpace::Screen );

	TSubclassOf<UCUserWidget_Health> healthClass;
	CHelpers::GetClass<UCUserWidget_Health>( &healthClass, "WidgetBlueprint'/Game/Widgets/WB_Health.WB_Health_C'" );
	HealthWidget->SetWidgetClass( healthClass );
	HealthWidget->SetRelativeLocation( FVector( 0, 0, 200 ) );
	HealthWidget->SetDrawSize( FVector2D( 120, 20 ) );
	HealthWidget->SetWidgetSpace( EWidgetSpace::Screen );
}

void ACEnemy::BeginPlay()
{
	UMaterialInstanceConstant* body;
	UMaterialInstanceConstant* logo;

	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>( &body, "MaterialInstanceConstant'/Game/Materials/M_UE4Man_Body_Inst.M_UE4Man_Body_Inst'" );
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>( &logo, "MaterialInstanceConstant'/Game/Materials/M_UE4Man_ChestLogo_Inst.M_UE4Man_ChestLogo_Inst'" );

	BodyMaterial = UMaterialInstanceDynamic::Create( body, this );
	LogoMaterial = UMaterialInstanceDynamic::Create( logo, this );

	GetMesh()->SetMaterial( 0, BodyMaterial );
	GetMesh()->SetMaterial( 1, LogoMaterial );

	Super::BeginPlay();

	State->OnStateTypeChanged.AddDynamic( this, &ACEnemy::OnStateTypeChanged );

	NameWidget->InitWidget();
	Cast<UCUserWidget_Name>( NameWidget->GetUserWidgetObject() )->SetNameText( GetActorLabel() );
	HealthWidget->InitWidget();
	Cast<UCUserWidget_Health>( HealthWidget->GetUserWidgetObject() )->UpdateHealth( Status->GetHealth(), Status->GetMaxHealth() );

	Action->SetUnarmedMode();
}

void ACEnemy::OnStateTypeChanged( EStateType InPrevType, EStateType InNewType )
{
	switch ( InNewType )
	{
	case EStateType::Idle:
		break;
	case EStateType::Roll:
		break;
	case EStateType::Backstep:
		break;
	case EStateType::Equip:
		break;
	case EStateType::Unequip:
		break;
	case EStateType::Action:
		break;
	case EStateType::Hitted:
		Hitted();
		break;
	case EStateType::Max:
		break;
	default:
		break;
	}
}

void ACEnemy::Hitted()
{
	Status->SubHealth( DamageValue );
	Cast<UCUserWidget_Health>( HealthWidget->GetUserWidgetObject() )->UpdateHealth( Status->GetHealth(), Status->GetMaxHealth() );
	DamageValue = 0.0f;

	Status->SetStop();
	Montages->PlayHitted();

	FVector start = GetActorLocation();
	FVector target = DamageInstigator->GetPawn()->GetActorLocation();

	SetActorRotation( UKismetMathLibrary::FindLookAtRotation( start, target ) );
	DamageInstigator = NULL;

	FVector direction = target - start;
	direction.Normalize();
	LaunchCharacter( -direction * LaunchAmount, true, false );

	ChangeColor(FLinearColor(1, 0, 0, 1));
	UKismetSystemLibrary::K2_SetTimer(this, "RestoreColor", 0.1f, false);
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACEnemy::ChangeColor( FLinearColor InColor )
{
	BodyMaterial->SetVectorParameterValue( "BodyColor", InColor );
	LogoMaterial->SetVectorParameterValue( "BodyColor", InColor );
}

float ACEnemy::TakeDamage( float Damage, FDamageEvent const& DamgeEvent, AController* EventInstigator, AActor* DamageCauser )
{
	//Super::TakeDamage( Damage, DamgeEvent, EventInstigator, DamageCauser );

	//CLog::Log( Damage );
	//ChangeColor( FLinearColor::Red );

	//return 0.0f;

	DamageInstigator = EventInstigator;
	DamageValue = Super::TakeDamage( Damage, DamgeEvent, EventInstigator, DamageCauser );

	State->SetHittedMode();
	return Status->GetHealth();
}

void ACEnemy::RestoreColor()
{
	FLinearColor color = Action->GetCurrent()->GetEquipmentColor();
	ChangeColor(color);
}