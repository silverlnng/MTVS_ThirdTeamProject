// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpWidget.h"

#include "HttpActor.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UHttpWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Btn_JsonRequest->OnClicked.AddDynamic(this,&UHttpWidget::OnMyClickSend);
	Btn_JsonPost->OnClicked.AddDynamic(this,&UHttpWidget::OnMyClickPost);
	Btn_GetWebImage->OnClicked.AddDynamic(this,&UHttpWidget::OnMyClickGetWebImage);
	Btn_JsonPost->SetVisibility(ESlateVisibility::Hidden);
	Btn_GetWebImage->SetVisibility(ESlateVisibility::Hidden);
	TextLog->SetVisibility(ESlateVisibility::Hidden);
	TodayPanel->SetVisibility(ESlateVisibility::Hidden);
}

void UHttpWidget::OnMyClickSend()
{
	FString fullURL = FString::Printf(TEXT("%s"), *URL);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *fullURL);

	httpActor->ReqStorageInfo(fullURL);
	
	if(TodayPanel->IsVisible())
	{
		TodayPanel->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		TodayPanel->SetVisibility(ESlateVisibility::Visible);
	}
	
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
	TextLog->SetText(FText::FromString(log));
}

void UHttpWidget::SetTextLogParse(FString logParse)
{
	TextLogParse->SetText(FText::FromString(logParse));
}

void UHttpWidget::SetHttpActor(class AHttpActor* actor)
{
	httpActor = actor;	
}

void UHttpWidget::SetWeatherImage(FString str)
{
	UTexture2D** FoundValue = WeatherData.Find(str);
	if(FoundValue==nullptr)
	{
		Img_WeatherImage->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	UTexture2D* img = WeatherData[str];
	if (img)
	{
		Img_WeatherImage->SetVisibility(ESlateVisibility::Visible);
		Img_WeatherImage->SetBrushFromTexture(img);
	}
	
	//WeatherData.Contains()
	
	
}
