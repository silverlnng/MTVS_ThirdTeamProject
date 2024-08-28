// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class MTVSTHIRDPROJECT_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
public:	
	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UButton* btn_create;


	class UNetWorkGameInstance* gi;
	//게임프레임워크 시스템은 줄여서 변수명 자주씀!

	UFUNCTION()
	void OnClickedCreateButton();
	
};
