// Fill out your copyright notice in the Description page of Project Settings.


#include "AJH_MainWidget.h"
#include "AJH_WeatherHttpActor.h"
#include "Components/Button.h"

void UAJH_MainWidget::NativeConstruct()
{
	Btn_Http->OnClicked.AddDynamic(this, &UAJH_MainWidget::OnClickedBtn_Http);
}

void UAJH_MainWidget::OnClickedBtn_Http()
{
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AAJH_WeatherHttpActor>(httpFactory, params);
	UE_LOG(LogTemp, Warning, TEXT("Click"));
}
