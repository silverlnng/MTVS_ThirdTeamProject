// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectRegionWidget.h"

#include "AJH_MainWidget.h"
#include "AJH_Player.h"
#include "YJHUD.h"
#include "YJPlayerController.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"

void USelectRegionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_Move->OnClicked.AddDynamic(this,&USelectRegionWidget::OnClickedBtnMove);
	
}

void USelectRegionWidget::OnClickedBtnMove()
{
	FString RegionString = ComboBox_Region->GetSelectedOption();
	// 현재 ComboBox_Region에 있는 값을 가져와서 regionMap 에서 찾아서
	auto value=regionMap.Find(RegionString);
	if(value==nullptr) {return;}

	AYJPlayerController* pc =GetOwningPlayer<AYJPlayerController>();
	if (pc)
	{
		AAJH_Player* palyer =pc->GetPawn<AAJH_Player>();
		if (palyer)
		{
			palyer->SetActorLocation(*value);
		}
		
		AYJHUD* hud = pc->GetHUD<AYJHUD>();
		if (hud)
		{
			hud->MainUI->SetVisibility(ESlateVisibility::Visible); 
		}
	}
	this->SetVisibility(ESlateVisibility::Hidden);
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle,[this]()
	{
		this->Destruct();
	},0.5f,false);
	// 창닫기
	
}
