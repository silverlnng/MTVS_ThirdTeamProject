﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
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
			InputMappingContext 모음
	***********************************************/
	UPROPERTY(EditDefaultsOnly)
	class UInputMappingContext* IMC_Operation;

	/***********************************************
			InputAction 변수 모음
	***********************************************/
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Move;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Interation;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Action;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Tap;

	/************************************************
			InputAction 함수 모음
	************************************************/
	void OnMyActionMove(const FInputActionValue& value);
	//void OnMyActionMove(float value);
	void OnMyActionInteration(const FInputActionValue& value);
	void OnMyAction(const FInputActionValue& value);
	void OnMyActionTap();
	/************************************************
			InputAction_RPC 함수 모음
	************************************************/
	UFUNCTION(Server, Reliable)
	void ServerOnMyAction(const FHitResult& outHit_);
	UFUNCTION(NetMulticast, Reliable)
	void MultiOnMyAction();

	UFUNCTION(NetMulticast, Reliable)
	void MultiOnMyActionPlayAnim();
	
	/************************************************
			마우스 포인트 상호작용
	************************************************/
	void MouseCusorEvent();

	/************************************************
			상호작용 범위 지정
	************************************************/
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* boxComp;

	float interationSize = 400;

	/************************************************
			플레이어 스테이터스
	************************************************/

	UPROPERTY(EditDefaultsOnly)
	float rotationSpeed = 15.0f;
	float RotationThreshold = 2.0f;

	/************************************************
			상호작용 라인트레이스
	************************************************/
	// 
	FVector worldLoc;
	// 
	FVector worldDir;
	// 
	FHitResult outHit;
	// 
	FVector start;
	// 
	FVector end;
	// 
	FCollisionQueryParams param;
	// 
	bool bHit;
	/************************************************
			라인트레이스 이벤트 함수
	************************************************/
	void InteractionLineTraceFuntion();

	/************************************************
			작물 및 InputAction
	************************************************/
	UPROPERTY(EditDefaultsOnly)
	class AAJH_FarmTile* farmTile;

	UPROPERTY(Replicated, EditDefaultsOnly)
	class AJS_ObstacleActor* object;


	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_SelectNone;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_SelectRiceSeed;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_SelectPumpkinSeed;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_SelectCarrotSeed;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_SelectWaterMelonSeed;
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_SelectStrawBerrySeed;

	ESeedType selectedSeedType;
	/************************
			씨앗 선택
	*************************/
	void OnMySelectedSeed(ESeedType newSeedType);
	void OnMySelectedNone();
	void OnMySelectRiceSeed();
	void OnMySelectPumpkinSeed();
	void OnMySelectCarrotSeed();
	void OnMySelectWaterMelonSeed();
	void OnMySelectStrawBerrySeed();
	/************************
			씨앗 행동
	*************************/
	void ActionNone();
	void ActionRice();
	void ActionPumpkin();
	void ActionCarrot();
	void ActionWaterMelon();
	void ActionStrawBerry();

	/************************
			작물 공장
	*************************/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AJS_SeedActor> riceFactory;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AJS_SeedActor> pumpkinFactory;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AJS_SeedActor> carrotFactory;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AJS_SeedActor> watermelonFatory;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AJS_SeedActor> strawberryFatory;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AJS_SeedActor> seedFatory;


	FActorSpawnParameters seedParam;
	UPROPERTY()
	class AJS_SeedActor* seed;
	UPROPERTY()
	class AJS_SecondRicePlant* harvest;

	/***********************************************
			미니맵 카메라
	***********************************************/

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<class ASceneCapture2D> miniMapCamera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class ASceneCapture2D* miniMapClass;
	UPROPERTY(EditDefaultsOnly)
	class UTextureRenderTarget2D* RT_Minimap;
	UPROPERTY(EditDefaultsOnly)
	class UMaterialInterface* miniMapMaterial;

	/************************************************
			오버랩 이벤트 함수
	************************************************/
	UFUNCTION()
	void OnMyBoxCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnMyBoxCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(Server, Reliable)
	void ServerBeginOverlap(AActor* OtherActor, bool bIsBeginOverlap);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastBeginOverlap(AActor* OtherActor, bool bIsBeginOverlap);
	UFUNCTION(Server, Reliable)
	void ServerEndOverlap(AActor* OtherActor, bool bIsBeginOverlap);
	UFUNCTION(NetMulticast, Reliable)
	void MulticastEndOverlap(AActor* OtherActor, bool bIsBeginOverlap);

	/************************
			애니메이션
	*************************/
	UPROPERTY(Replicated)
	class UAJH_PlayerAnimInstance* anim;

	/************************************************
			UI 정보 할당
	************************************************/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> weatherUI;
	UPROPERTY(EditDefaultsOnly)
	class UAJH_WeatherWidget* httpWeatherUI;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UHttpWidget> HttpUI_factory;
	UPROPERTY(EditDefaultsOnly)
	class UHttpWidget* HttpUI;
	
	
	/************************************************
			UI ����
	************************************************/
	UPROPERTY(EditDefaultsOnly)
	class UUserWidget* TapUI;

	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* UserNameWidgetComp;

	UPROPERTY(EditDefaultsOnly)
	class UUserNameWidget* UserNameUI;
	
	UPROPERTY(Replicated,EditDefaultsOnly)
	FString UserName;
	UPROPERTY(Replicated,EditDefaultsOnly)
	int32 MeshNum;
	UFUNCTION(server, reliable)
	void ServerChange(const FString& userName_, int32 meshNum_);

	UPROPERTY(BlueprintReadOnly)
	class UNetWorkGameInstance* gi;

	UPROPERTY(VisibleAnywhere)
	int32 CurQuestNum=0;

	UPROPERTY(VisibleAnywhere)
	class AYJPlayerController* pc;
	
	UPROPERTY(VisibleAnywhere)
	class AYJHUD* YJhud;
	UFUNCTION()
	void SuccessQuest();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
};

UENUM(BlueprintType)
enum class ESeedType : uint8
{
	None,
	RiceSeed,
	PumpkinSeed,
	CarrotSeed,
	WaterMelonSeed,
	StrawBerrySeed,
};

