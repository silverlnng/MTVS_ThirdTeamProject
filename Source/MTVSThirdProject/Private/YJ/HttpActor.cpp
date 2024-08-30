// Fill out your copyright notice in the Description page of Project Settings.


#include "YJ/HttpActor.h"
#include "HttpModule.h"
#include "ImageUtils.h"


// Sets default values
AHttpActor::AHttpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHttpActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHttpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHttpActor::ReqStorageInfo(FString url)
{
	// http 모듈을 생성
	FHttpModule& httpModule = FHttpModule::Get();

	TSharedRef<IHttpRequest> httpReq =httpModule.CreateRequest();

	//요청할 정보를 설정
	httpReq->SetURL(url);
	httpReq->SetVerb(TEXT("GET"));
	httpReq->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	httpReq->OnProcessRequestComplete().BindUObject(this,&AHttpActor::OnResStorageInfo);
	// FHttpRequestCompleteDelegate 으로 완료되면 실행할 함수를 bind (응답받을 함수를 연결)
	// 응답받을 함수를 연결

	//서버에 요청
	httpReq->ProcessRequest();
}

void AHttpActor::OnResStorageInfo(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void AHttpActor::ReqPostTest(const FString url, const FString& JSON)
{
	// http 모듈을 생성
	FHttpModule& httpModule = FHttpModule::Get();

	TSharedRef<IHttpRequest> httpReq =httpModule.CreateRequest();

	//요청할 정보를 설정
	httpReq->SetURL(url);
	httpReq->SetVerb(TEXT("POST"));
	httpReq->SetHeader(TEXT("Content-Type"), TEXT("application/json")); // Content-Type 이 JSON 을 사용한다는 것을 명시하기

	httpReq->SetContentAsString(JSON); // 추가


	httpReq->OnProcessRequestComplete().BindUObject(this,&AHttpActor::OnResPostTest);
	// FHttpRequestCompleteDelegate 으로 완료되면 실행할 함수를 bind (응답받을 함수를 연결)
	// 응답받을 함수를 연결

	//서버에 요청
	httpReq->ProcessRequest();
}

void AHttpActor::OnResPostTest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if(bConnectedSuccessfully) // 성공
	{
		FString result = Response->GetContentAsString();
	
		// fstring result 을받아서 활용하기
		// HttpUI->SetTextLog(result); //문자열로 받아서
	
	}
	else //실패
	{
		UE_LOG(LogTemp,Warning,TEXT("OnResPostTest Failed"));
	}
}

void AHttpActor::ReqGetWebImageToServer(FString url, TArray<uint8> resouce)
{
}

void AHttpActor::OnResGetWebImageFromServer(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bConnectedSuccessfully)
{
}

