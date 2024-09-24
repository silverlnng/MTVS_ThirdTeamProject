// Fill out your copyright notice in the Description page of Project Settings.


#include "AJH_MainWidget.h"
#include "AJH_WeatherHttpActor.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UAJH_MainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Button_35->SetVisibility(ESlateVisibility::Hidden);
}

void UAJH_MainWidget::SetTextQuestInfo(FString str)
{
	Text_QuestInfo->SetText(FText::FromString(str));
}

