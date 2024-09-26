// Fill out your copyright notice in the Description page of Project Settings.


#include "YJHUD.h"

#include "AJH_MainWidget.h"
#include "NPCWidget.h"
#include "SelectRegionWidget.h"
#include "Blueprint/UserWidget.h"

void AYJHUD::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwningPlayerController()->IsLocalController())
	{
		MainUI =CreateWidget<UAJH_MainWidget>(GetWorld(),MainWidgetFactory);
		if(MainUI)
		{
			MainUI->AddToViewport();
			MainUI->SetVisibility(ESlateVisibility::Visible);
		}
		NPCUI =CreateWidget<UNPCWidget>(GetWorld(),NPCWidgetFactory);
		
		if(NPCUI)
		{
			NPCUI->AddToViewport();
			NPCUI->SetVisibility(ESlateVisibility::Hidden);
		}
		
		
		SelectRegionUI =CreateWidget<USelectRegionWidget>(GetWorld(),SelectRegionWidgetFactory);
		
		if(SelectRegionUI)
		{
			SelectRegionUI->AddToViewport();
			SelectRegionUI->SetVisibility(ESlateVisibility::Hidden);
		}
		
	}
	
	
}
