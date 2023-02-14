#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/CStateComponent.h"
#include "Engine/DataTable.h"	// Engine의 Datatable사용을 위한 헤더

#include "CMontagesComponent.generated.h"


// Montage 정보를 저장할 struct 생성
USTRUCT( BlueprintType )
struct FMontageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY( EditAnywhere )
		EStateType Type;
	UPROPERTY( EditAnywhere )
		class UAnimMontage* AnimMontage;
	UPROPERTY( EditAnywhere )
		float PlayRatio = 1.0f;
	UPROPERTY( EditAnywhere )
		FName StartSection;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPROJ2_API UCMontagesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCMontagesComponent();

	void PlayRoll();
	void PlayBackstep();
	void PlayHitted();

protected:
	virtual void BeginPlay() override;

private:
	void PlayAnimMontage( EStateType InType );

private:
	UPROPERTY( EditDefaultsOnly, Category = "DataTable" )
		UDataTable* DataTable;

	FMontageData* Datas[(int32)EStateType::Max];

};
