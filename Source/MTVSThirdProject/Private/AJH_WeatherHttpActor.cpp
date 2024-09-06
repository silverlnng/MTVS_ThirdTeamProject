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

	// UI�� �����ؼ� ����ϰ� �ʹ�
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
	// Http����� ����
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	// ��û�� ������ ����
	req->SetURL(url);
	req->SetVerb(TEXT("GET"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	// ������� �Լ��� ����
	req->OnProcessRequestComplete().BindUObject(this, &AAJH_WeatherHttpActor::OnResTodayWeather);
	// ������ ��û

	req->ProcessRequest();
	UE_LOG(LogTemp, Warning, TEXT("Request Sent : %s"), *url);
}

void AAJH_WeatherHttpActor::OnResTodayWeather(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		// ����
		FString result = Response->GetContentAsString();

		// �ʿ��� ������ �̾Ƽ� ȭ�鿡 ����ϰ�ʹ�.
		httpWeatherUI->SetTextLog(UAJH_JsonParseLib::WeatherJsonParse(result));
	}
	else {
		// ����
		UE_LOG(LogTemp, Warning, TEXT("OnResNewBookInfo Failed..."));
	}
}

