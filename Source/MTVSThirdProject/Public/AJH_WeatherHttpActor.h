// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "AJH_WeatherHttpActor.generated.h"

UCLASS()
class MTVSTHIRDPROJECT_API AAJH_WeatherHttpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAJH_WeatherHttpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/************************************************
			웹 통신 함수
	************************************************/
	UPROPERTY(EDitDefaultsOnly)
	TSubclassOf<class UUserWidget> httpUIFactory;

	UPROPERTY()
	class UAJH_WeatherWidget* httpWeatherUI;
	
	// BT_today 를 눌렀을 때 요청 할 함수
	void ReqTodayWeather(FString url);
	// 응답 받을 함수
	void OnResTodayWeather(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);


};
