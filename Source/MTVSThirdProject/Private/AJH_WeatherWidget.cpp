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
	BT_Tomorrow->OnClicked.AddDynamic(this, &UAJH_WeatherWidget::OnMyClickTomorrow);
}

void UAJH_WeatherWidget::OnMyClickToday()
{
	TMap<FString, FString> todayData;
	todayData.Add("Type", "today");
	FString json = UAJH_JsonParseLib::MakeWeatherJson(todayData);
	//FString json = AAJH_Player::

	playerhttp->ReqTodayWeather(ServerURL, json);

	UE_LOG(LogTemp, Warning, TEXT("Today's weather request sent."));
}

void UAJH_WeatherWidget::OnMyClickTomorrow()
{
	// 내일의 날씨 요청
	TMap<FString, FString> tomorrowData;
	tomorrowData.Add("type", "tomorrow");
	FString json = UAJH_JsonParseLib::MakeWeatherJson(tomorrowData);

	// 플레이어 클래스의 ReqTodayWeather 함수를 호출하여 서버에 요청
	if (playerhttp)
	{
		playerhttp->ReqTodayWeather(ServerURL, json);
	}

	UE_LOG(LogTemp, Warning, TEXT("Tomorrow's weather request sent."));
}

void UAJH_WeatherWidget::SetTextLog(FString log)
{
	T_TextLog->SetText(FText::FromString(log));
}
