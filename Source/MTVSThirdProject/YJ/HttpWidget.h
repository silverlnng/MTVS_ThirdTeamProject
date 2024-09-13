// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HttpWidget.generated.h"

/**
 * 
 */
UCLASS()
class MTVSTHIRDPROJECT_API UHttpWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY()
	class AHttpActor* httpActor;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_JsonRequest;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_JsonPost;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_GetWebImage;

	UFUNCTION()
	void OnMyClickSend();
	
    UFUNCTION()
    void OnMyClickPost(); 
    
    UFUNCTION()
    void OnMyClickGetWebImage();
    
    UFUNCTION()
    void SetWebImage(class UTexture2D* newTexture);

	void SetTextLog(FString log);

	void SetTextLogParse(FString logParse);

	void SetHttpActor(class AHttpActor* actor);
	FString URL = "https://safe-quickly-lioness.ngrok-free.app/weather/Seoul";
		
    //FString URL = "gs://unrealthirdteamproject.appspot.com"; // 파이어베이스 storage 버킷
    //FString Key = "AIzaSyD2-X2ghnSSC4tW-EmoaanENN9nMoMFKuM"; // 웹 ApI 키
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TextLog;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TextLogParse;

	UPROPERTY(meta=(BindWidget))
	class UImage* Img_WeatherImage;

	UFUNCTION()
	void SetWeatherImage(FString str);

	UPROPERTY(EditAnywhere)
	TMap<FString,UTexture2D*> WeatherData;
	
};
