// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_SetNickName.generated.h"
#include "Components/EditableTextBox.h"
/**
 * 
 */
UCLASS()
class UNREALPROJ2_API UCUserWidget_SetNickName : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY( BlueprintReadWrite, EditAnywhere )
		class UButton* EnterButton;
	UPROPERTY( BlueprintReadWrite, EditAnywhere )
		class UEditableTextBox* UserNameTextBox;

	FString UserName;


	UFUNCTION( BlueprintCallable )
		FString LoadUserName();
protected:
	UFUNCTION( BlueprintCallable )
		void OnEnterButton();
};
