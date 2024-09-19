// Fill out your copyright notice in the Description page of Project Settings.


#include "AJH_PlayerAnimInstance.h"
#include "AJH_Player.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAJH_PlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	player = Cast<AAJH_Player>(TryGetPawnOwner());
	if (player == nullptr)
	{
		return;
	}

	FVector velocity = player->GetVelocity();

	FVector forwardVector = player->GetActorForwardVector();
	speed = FVector::DotProduct(forwardVector, velocity);

	FVector rightVector = player->GetActorRightVector();
	direction = FVector::DotProduct(rightVector, velocity);

}

void UAJH_PlayerAnimInstance::PlayMeleeAttackMontage()
{
	bEndAnimation = true;
	bAttackAnimation = true;
	if (meleeAttackMontage)
	{
		Montage_Play(meleeAttackMontage);
		player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	}
}

void UAJH_PlayerAnimInstance::OnEndAnim()
{
	bEndAnimation = false;
	bAttackAnimation = false;
	player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

