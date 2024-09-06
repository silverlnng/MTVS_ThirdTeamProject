// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpActor.h"
#include "HttpModule.h"
#include "HttpWidget.h"
#include "ImageUtils.h"
#include "JsonParseLib.h"


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
	HttpUI = Cast<UHttpWidget>(CreateWidget(GetWorld(), HttpUIFactory));
	if (HttpUI)
	{
		HttpUI->AddToViewport();
		HttpUI->SetHttpActor(this);
	}

	auto* pc = GetWorld()->GetFirstPlayerController();
	pc->SetShowMouseCursor(true);
	pc->SetInputMode(FInputModeUIOnly());
}

// Called every frame
void AHttpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHttpActor::ReqStorageInfo(FString url) //요청하는 함수
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
	if (bConnectedSuccessfully)
	{
		// 성공
		FString result = Response->GetContentAsString();

		// 필요한 정보만 뽑아서 화면에 출력하고싶다.
		//HttpUI->SetTextLog(UJsonParseLib::JsonParse(result));
		HttpUI->SetTextLog(result);
		HttpUI->SetTextLogParse(UJsonParseLib::JsonParse(result));
	}
	else{
		// 실패
		UE_LOG(LogTemp, Warning, TEXT("OnResNewBookInfo Failed..."));
	}
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
		 HttpUI->SetTextLog(result); //문자열로 받아서
	}
	else //실패
	{
		UE_LOG(LogTemp,Warning,TEXT("OnResPostTest Failed"));
	}
}

void AHttpActor::ReqGetWebImage(FString url)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	// 요청할 정보를 설정
	req->SetURL(url);
	req->SetVerb(TEXT("GET"));
	req->SetHeader(TEXT("content-type"), TEXT("image/jpeg"));

	// 응답받을 함수를 연결
	req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResGetWebImage);
	// 서버에 요청

	req->ProcessRequest();
}

void AHttpActor::OnResGetWebImage(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		TArray<uint8> data = Response->GetContent();
		FString fileServerURL = "http://mtvs.helloworldlabs.kr:7771/api/byte";
		ReqGetWebImageToServer(fileServerURL, data);
	}
	else {
		// 실패
		UE_LOG(LogTemp, Warning, TEXT("OnResGetWebImage Failed..."));
	}
}

void AHttpActor::ReqGetWebImageToServer(FString url, TArray<uint8> resouce)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	// 요청할 정보를 설정
	req->SetURL(url);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/octet-stream"));
	req->SetContent(resouce);

	// 응답받을 함수를 연결
	req->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResGetWebImageFromServer);
	// 서버에 요청

	req->ProcessRequest();
}

void AHttpActor::OnResGetWebImageFromServer(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		TArray<uint8> data = Response->GetContent();
		FString imagePath = FPaths::ProjectPersistentDownloadDir()+"/Cat.jpg";
		FFileHelper::SaveArrayToFile(data, *imagePath);
		UTexture2D* realTexture = FImageUtils::ImportBufferAsTexture2D(data);
		HttpUI->SetWebImage(realTexture);
	}
	else {
		// 실패
		UE_LOG(LogTemp, Warning, TEXT("OnResGetWebImageFromServer Failed..."));
	}
}

