// Fill out your copyright notice in the Description page of Project Settings.


#include "AJH_PlayerController.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "AJH_Player.h"

AAJH_PlayerController::AAJH_PlayerController()
{

}

void AAJH_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	me = Cast<AAJH_Player>(GetOwner());

}

void AAJH_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	

}

void AAJH_PlayerController::OnMyActionMove(const FInputActionValue& value)
{
	
}
