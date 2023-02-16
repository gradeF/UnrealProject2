// Fill out your copyright notice in the Description page of Project Settings.


#include "CUserWidget_SetNickName.h"
#include "Global.h"


FString UCUserWidget_SetNickName::LoadUserName()
{
	return UserName;
}

void UCUserWidget_SetNickName::OnEnterButton()
{
	if (UserNameTextBox != nullptr)
	{
		UserName = UserNameTextBox->GetText().ToString();
		FText UserNameText = FText::FromString( UserName );
	}
}
