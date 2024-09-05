// Fill out your copyright notice in the Description page of Project Settings.


#include "UserNameWidget.h"

#include "Components/TextBlock.h"

void UUserNameWidget::SetUserName(FString value)
{
	Text_UserName->SetText(FText::FromString(value));
}
