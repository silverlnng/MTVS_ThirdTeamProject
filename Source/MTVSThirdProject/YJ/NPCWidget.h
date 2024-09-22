// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NPCWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReadCSV,int32,Num);

UCLASS()
class MTVSTHIRDPROJECT_API UNPCWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	FReadCSV ReadCSVDele;
	UPROPERTY(VisibleAnywhere)
	class AYJPlayerController* YJPC;
	
	UPROPERTY(VisibleAnywhere)
	int32 curCount =0;

	UPROPERTY(meta=(BindWidget))
	class UButton* backButton;
	
	UPROPERTY(meta=(BindWidget))
	class UButton* frontButton;
	
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* TextLog;

	UPROPERTY(meta=(BindWidget))
	class UHorizontalBox* YesNoBox;

	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_Yes;

	UPROPERTY(meta=(BindWidget))
	class UButton* Btn_No;
	
	
	UFUNCTION()
	void SetTextLog(FString str);

	UFUNCTION()
	void OnClickbackButton();
	UFUNCTION()
	void OnClickfrontButton();

	UFUNCTION()
	void OnClickYesBtn();
	UFUNCTION()
	void OnClickNoBtn();
	
};
