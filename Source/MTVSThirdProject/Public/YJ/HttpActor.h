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

	// ui send버튼을 눌렀을때 요청할 함수
	void ReqStorageInfo(FString url);

	void OnResStorageInfo(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully); // 응답받을 함수
    
    void ReqPostTest(const FString url, const FString& JSON);
    
    void OnResPostTest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);// 응답받을 함수   
    
    void ReqGetWebImageToServer(FString url,TArray<uint8> resouce);
    void OnResGetWebImageFromServer(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	
};
