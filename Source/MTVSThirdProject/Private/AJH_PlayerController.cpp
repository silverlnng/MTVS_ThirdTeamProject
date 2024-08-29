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

	UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (subSys)
	{
		subSys->AddMappingContext(IMC_Operation, 0);
	}
	

	auto input = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (input)
	{
		input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AAJH_PlayerController::OnMyActionMove);
	}

}

void AAJH_PlayerController::OnMyActionMove(const FInputActionValue& value)
{
	FVector2D MovementVector = value.Get<FVector2D>();

	if (this != nullptr)
	{
		const FRotator Rotation = this->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		me->AddMovementInput(ForwardDirection, MovementVector.Y);
		me->AddMovementInput(RightDirection, MovementVector.X);
	}
}
