// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AJH_WeatherWidget.generated.h"

/**
 * 
 */
UCLASS()
class MTVSTHIRDPROJECT_API UAJH_WeatherWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	class UButton* BT_today;
	UFUNCTION()
	void OnMyClickToday();
	UPROPERTY(meta=(BindWidget))
	class UButton* BT_Tomorrow;
	UFUNCTION()
	void OnMyClickTomorrow();
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* T_TextLog;
	void SetTextLog(FString log);

	UPROPERTY()
	class AAJH_Player* playerhttp;
	
	FString ServerURL = "http://222.103.183.137:8000/weather/Seoul/";
	//FString ServerURL = "http://safe-quickly-lioness.ngrok-free.app/weather/Seoul";

};
