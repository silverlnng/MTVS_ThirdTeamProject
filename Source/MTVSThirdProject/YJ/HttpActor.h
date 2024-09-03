// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpActor.generated.h"

UCLASS()
class MTVSTHIRDPROJECT_API AHttpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHttpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 인터페이스를 위한 UI
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> HttpUIFactory;

	UPROPERTY()
	class UHttpWidget* HttpUI;

	// ui send버튼을 눌렀을때 요청할 함수
	void ReqStorageInfo(FString url);

	void OnResStorageInfo(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully); // 응답받을 함수
    
    void ReqPostTest(const FString url, const FString& JSON);
    
    void OnResPostTest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);// 응답받을 함수

	
	// ButtonGetWebImage를 눌렀을 때 요청할 함수
	void ReqGetWebImage(FString url);
	// 응답받을 함수
	void OnResGetWebImage(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// 웹이미지를 백엔드 서버로 송신
    void ReqGetWebImageToServer(FString url,TArray<uint8> resouce);
	// 웹이미지를 백엔드 서버에서 수신
    void OnResGetWebImageFromServer(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	
};
