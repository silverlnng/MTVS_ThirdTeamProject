// Fill out your copyright notice in the Description page of Project Settings.


#include "AJH_WeatherWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "AJH_JsonParseLib.h"
#include "AJH_Player.h"

void UAJH_WeatherWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BT_today->OnClicked.AddDynamic(this, &UAJH_WeatherWidget::OnMyClickToday);
}

void UAJH_WeatherWidget::OnMyClickToday()
{
	TMap<FString, FString> todayData;
	todayData.Add("today", "today");
	todayData.Add("temperature", "temperature");
	todayData.Add("weather", "weather");
	todayData.Add("humidity", "humidity");
	todayData.Add("windSpeed", "windSpeed");
	FString json = UAJH_JsonParseLib::MakeWeatherJson(todayData);

	playerhttp->ReqTodayWeather(ServerURL, json);
	UE_LOG(LogTemp, Warning, TEXT("Successed"));
}

void UAJH_WeatherWidget::OnMyClickTomorrow()
{

}

void UAJH_WeatherWidget::SetTextLog(FString log)
{
	T_TextLog->SetText(FText::FromString(log));
}
