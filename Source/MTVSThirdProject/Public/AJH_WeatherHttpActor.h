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
			�� ��� �Լ�
	************************************************/
	UPROPERTY(EDitDefaultsOnly)
	TSubclassOf<class UUserWidget> httpUIFactory;

	UPROPERTY()
	class UAJH_WeatherWidget* httpWeatherUI;
	
	// BT_today �� ������ �� ��û �� �Լ�
	void ReqTodayWeather(FString url);
	// ���� ���� �Լ�
	void OnResTodayWeather(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	
};
