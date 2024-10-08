﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AJH_MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class MTVSTHIRDPROJECT_API UAJH_MainWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_QuestInfo;

	UPROPERTY(meta=(BindWidget))
	class UImage* Img_Minimap;
	
	void SetTextQuestInfo(FString str);
};
