// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "AJH_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MTVSTHIRDPROJECT_API AAJH_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAJH_PlayerController();


	virtual void SetupInputComponent() override;

	// To add mapping context
	virtual void BeginPlay();

	/***********************************************
			InputMappingContext ����
	***********************************************/
	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* IMC_Operation;

	/***********************************************
			InputAction ���� ����
	***********************************************/
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Move;

	/************************************************
			InputAction �Լ� ����
	************************************************/
	void OnMyActionMove(const FInputActionValue& value);

	/************************************************
			Owner ����
	************************************************/
	UPROPERTY()
	class AAJH_Player* me;

};
