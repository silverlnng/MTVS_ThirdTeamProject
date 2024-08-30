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

    FString URL = "gs://unrealthirdteamproject.appspot.com"; // 파이어베이스 storage 버킷
    FString Key = "AIzaSyD2-X2ghnSSC4tW-EmoaanENN9nMoMFKuM"; // 웹 ApI 키 
	
};
