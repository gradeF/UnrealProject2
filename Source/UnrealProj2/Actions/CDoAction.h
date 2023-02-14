#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actions/CActionData.h"
#include "CDoAction.generated.h"

UCLASS()
class UNREALPROJ2_API ACDoAction : public AActor
{
	GENERATED_BODY()
	
public:	
	ACDoAction();
	virtual void DoAction(){}
	virtual void Begin_DoAction(){}
	virtual void End_DoAction(){}

	FORCEINLINE void SetDatas( TArray<FDoActionData> InDatas ) { Datas = InDatas; }

	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(
			class ACharacter* InAttacker,
			class AActor* InAttackCauser,
			class ACharacter* InOtherCharacter
		){}
	UFUNCTION()
		virtual void OnAttachmentEndOverlap(
			class ACharacter* InAttacker,
			class AActor* InAttackCauser,
			class ACharacter* InOtherCharacter
		) {}

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY( BlueprintReadOnly )
		class ACharacter* OwnerCharacter;
	UPROPERTY( BlueprintReadOnly )
		class UCStateComponent* State;
	UPROPERTY( BlueprintReadOnly )
		class UCStatusComponent* Status;


protected:
	TArray<FDoActionData> Datas;


};
