// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "AJH_Player.generated.h"

UCLASS()
class MTVSTHIRDPROJECT_API AAJH_Player : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAJH_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* springArmComp;
	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* cameraComp;

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
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Interation;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Action;

	/************************************************
			InputAction �Լ� ����
	************************************************/
	void OnMyActionMove(const FInputActionValue& value);
	void OnMyActionInteration(const FInputActionValue& value);
	void OnMyAction(const FInputActionValue& value);

	/************************************************
			���콺 �̺�Ʈ ó�� �Լ�
	************************************************/
	void MouseCusorEvent();

	/************************************************
			ĳ���� ��ȣ�ۿ� ����
	************************************************/
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* boxComp;

	float interationSize = 400;

	/************************************************
			����Ʈ���̽� ����
	************************************************/
	// ���콺�� ��ġ
	FVector worldLoc;
	// ���콺�� ����
	FVector worldDir;
	// ���� Ʈ���̽� ���
	FHitResult outHit;
	// ��ŸƮ ����
	FVector start;
	// ���� ����
	FVector end;
	// ����ڵ�
	FCollisionQueryParams param;
	// �浹 �Ǵ� ����
	bool bHit;
	/************************************************
			����Ʈ���̽� �Լ�
	************************************************/
	void InteractionLineTraceFuntion();

	/************************************************
			���� �Ҵ� ����
	************************************************/
	UPROPERTY(EditDefaultsOnly)
	class AAJH_FarmTile* farmTile;


	/************************************************
			������ �̺�Ʈ �Լ�
	************************************************/
	UFUNCTION()
	void OnMyBoxCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnMyBoxCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
