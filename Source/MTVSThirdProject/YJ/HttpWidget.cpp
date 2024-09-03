// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpWidget.h"

#include "Components/Button.h"

void UHttpWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_JsonRequest->OnClicked.AddDynamic(this,&UHttpWidget::OnMyClickSend);
	Btn_JsonPost->OnClicked.AddDynamic(this,&UHttpWidget::OnMyClickPost);
	Btn_GetWebImage->OnClicked.AddDynamic(this,&UHttpWidget::OnMyClickGetWebImage);
}

void UHttpWidget::OnMyClickSend()
{
	
}

void UHttpWidget::OnMyClickPost()
{
	TMap<FString,FString> StudentData;

	StudentData.Add("Name","Kei");
	StudentData.Add("Age","23");
	StudentData.Add("Height","185");

	/*FString json =UJsonParseLib::MakeJson(StudentData);

	httpActor->ReqPostTest(ServerURL,json);*/
}

void UHttpWidget::OnMyClickGetWebImage()
{
}

void UHttpWidget::SetWebImage(class UTexture2D* newTexture)
{
}

void UHttpWidget::SetTextLog(FString log)
{
}

void UHttpWidget::SetHttpActor(class AHttpActor* actor)
{
}
