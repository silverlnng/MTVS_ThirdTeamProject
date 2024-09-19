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
			�ִϸ��̼� ����
	***********************************************/
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class UAnimMontage* meleeAttackMontage;
	void PlayMeleeAttackMontage();
	// ��Ƽ���̷� ���� �غ��� ��ŸƮ�� inpt�Է� ���� �ʰ� ����� input�Է� �ް�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bEndAnimation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bAttackAnimation = false;
	UFUNCTION(BlueprintCallable)
	void OnEndAnim();

};
