// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AJH_PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MTVSTHIRDPROJECT_API UAJH_PlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Vertical
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float speed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float direction;


};
