// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"

#include "NetWorkGameInstance.h"
#include "Components/Button.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();
	btn_create->OnClicked.AddDynamic(this,&ULoginWidget::OnClickedCreateButton);
	gi = GetGameInstance<UNetWorkGameInstance>();//게임프레임워크는 가져오기 쉬움
}

void ULoginWidget::OnClickedCreateButton()
{
	if(gi!=nullptr)
	{
		gi->CreateMySession(TEXT(""), TEXT(""), 2);
	}
}
