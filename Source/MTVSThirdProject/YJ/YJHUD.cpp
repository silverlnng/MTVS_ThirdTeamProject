// Fill out your copyright notice in the Description page of Project Settings.


#include "YJHUD.h"

#include "NPCWidget.h"
#include "Blueprint/UserWidget.h"

void AYJHUD::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwningPlayerController()->IsLocalController())
	{
		NPCUI =CreateWidget<UNPCWidget>(GetWorld(),NPCWidgetFactory);
		
		if(NPCUI)
		{
			NPCUI->AddToViewport();
			NPCUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
	
}
