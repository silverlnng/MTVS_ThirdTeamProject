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
	// Horizontal
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float direction;

	// Owner
	UPROPERTY()
	class AAJH_Player* player;
	
	/***********************************************
			애니메이션 모음
	***********************************************/
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UAnimMontage* meleeAttackMontage;
	void PlayMeleeAttackMontage();
	// 노티파이로 제어 해보자 스타트면 inpt입력 받지 않게 엔드면 input입력 받게
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bEndAnimation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bAttackAnimation = false;
	UFUNCTION(BlueprintCallable)
	void OnEndAnim();

};
