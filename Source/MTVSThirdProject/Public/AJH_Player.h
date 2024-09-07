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
			작물 정보 할당
	************************************************/
	UPROPERTY(EditDefaultsOnly)
	class AAJH_FarmTile* farmTile;

	UPROPERTY(EditDefaultsOnly)
	class AJS_Tree* tree;
	UPROPERTY(EditDefaultsOnly)
	class AJS_Rock* rock;
	UPROPERTY(EditDefaultsOnly)
	class AJS_Gress* gress;

	/************************************************
			오버랩 이벤트 함수
	************************************************/
	UFUNCTION()
	void OnMyBoxCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnMyBoxCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/************************************************
			UI 정보 할당
	************************************************/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> weatherUI;
	UPROPERTY(EditDefaultsOnly)
	class UAJH_WeatherWidget* httpWeatherUI;

	// BT_today 
	void ReqTodayWeather(FString url);

	// 
	void OnResTodayWeather(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

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
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
};
