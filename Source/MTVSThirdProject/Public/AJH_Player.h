// Fill out your copyright notice in the Description page of Project Settings.

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
	void OnMyActionInteration(const FInputActionValue& value);
	void OnMyAction(const FInputActionValue& value);
	void OnMyActionTap();

	/************************************************
			마우스 이벤트 처리 함수
	************************************************/
	void MouseCusorEvent();

	/************************************************
			캐릭터 상호작용 범위
	************************************************/
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* boxComp;

	float interationSize = 400;

	/************************************************
			라인트레이스 변수
	************************************************/
	// 마우스의 위치
	FVector worldLoc;
	// 마우스의 방향
	FVector worldDir;
	// 라인 트레이스 출력
	FHitResult outHit;
	// 스타트 지점
	FVector start;
	// 엔드 지점
	FVector end;
	// 방어코드
	FCollisionQueryParams param;
	// 충돌 판단 여부
	bool bHit;
	/************************************************
			라인트레이스 함수
	************************************************/
	void InteractionLineTraceFuntion();

	/************************************************
			정보 할당 모음
	************************************************/
	UPROPERTY(EditDefaultsOnly)
	class AAJH_FarmTile* farmTile;
	UPROPERTY(EditDefaultsOnly)
	class AJS_Tree* treeTile;

	/************************************************
			오버랩 이벤트 함수
	************************************************/
	UFUNCTION()
	void OnMyBoxCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnMyBoxCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/************************************************
			웹 통신 함수 및 UI 정보 할당
	************************************************/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> weatherUI;
	UPROPERTY(EditDefaultsOnly)
	class UAJH_WeatherWidget* httpWeatherUI;

	// BT_today 를 눌렀을 때 요청 할 함수
	void ReqTodayWeather(FString url);
	// 응답 받을 함수
	//UFUNCTION()
	void OnResTodayWeather(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	/************************************************
			UI 모음
	************************************************/
	UPROPERTY(EditDefaultsOnly)
	class UUserWidget* TapUI;

};
