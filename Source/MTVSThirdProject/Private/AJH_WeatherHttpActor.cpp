// Fill out your copyright notice in the Description page of Project Settings.


#include "AJH_WeatherHttpActor.h"
#include "HttpModule.h"
#include "AJH_WeatherWidget.h"
#include "AJH_Player.h"
#include "Kismet/GameplayStatics.h"
#include "AJH_JsonParseLib.h"

// Sets default values
AAJH_WeatherHttpActor::AAJH_WeatherHttpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAJH_WeatherHttpActor::BeginPlay()
{
	Super::BeginPlay();

	// UI를 생성해서 기억하고 싶다
	httpWeatherUI = Cast<UAJH_WeatherWidget>(CreateWidget(GetWorld(), httpUIFactory));
	if (httpWeatherUI)
	{
		httpWeatherUI->AddToViewport();
	}
	
}

// Called every frame
void AAJH_WeatherHttpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAJH_WeatherHttpActor::ReqTodayWeather(FString url)
{
	// Http모듈을 생성
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	// 요청할 정보를 설정
	req->SetURL(url);
	req->SetVerb(TEXT("GET"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	// 응답받을 함수를 연결
	req->OnProcessRequestComplete().BindUObject(this, &AAJH_WeatherHttpActor::OnResTodayWeather);
	// 서버에 요청

	req->ProcessRequest();
	UE_LOG(LogTemp, Warning, TEXT("Request Sent : %s"), *url);
}

void AAJH_WeatherHttpActor::OnResTodayWeather(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		// 성공
		FString result = Response->GetContentAsString();

		// 필요한 정보만 뽑아서 화면에 출력하고싶다.
		httpWeatherUI->SetTextLog(UAJH_JsonParseLib::WeatherJsonParse(result));
	}
	else {
		// 실패
		UE_LOG(LogTemp, Warning, TEXT("OnResNewBookInfo Failed..."));
	}
}

