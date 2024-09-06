// Fill out your copyright notice in the Description page of Project Settings.


#include "AJH_Player.h"
#include "AJH_FarmTile.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "HttpModule.h"
#include "AJH_WeatherWidget.h"
#include "JS_Tree.h"


// Sets default values
AAJH_Player::AAJH_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeRotation(FRotator(-55, 0, 0));
	springArmComp->TargetArmLength = 650;

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	cameraComp->SetupAttachment(springArmComp);

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	boxComp->SetupAttachment(RootComponent);
	boxComp->SetBoxExtent(FVector(200, 200, 13));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempBodyMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (tempBodyMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempBodyMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}
}

// Called when the game starts or when spawned
void AAJH_Player::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AAJH_Player::OnMyBoxCompBeginOverlap);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &AAJH_Player::OnMyBoxCompEndOverlap);
	
	
}

// Called every frame
void AAJH_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MouseCusorEvent();
	
}

// Called to bind functionality to input
void AAJH_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		pc->SetShowMouseCursor(true);
		UEnhancedInputLocalPlayerSubsystem* subSys = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subSys)
		{
			subSys->AddMappingContext(IMC_Operation, 0);
		}
	}

	auto input = CastChecked<UEnhancedInputComponent>(InputComponent);
	if (input)
	{
		input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AAJH_Player::OnMyActionMove);
		input->BindAction(IA_Interation, ETriggerEvent::Started, this, &AAJH_Player::OnMyActionInteration);
		input->BindAction(IA_Action, ETriggerEvent::Started, this, &AAJH_Player::OnMyAction);
		input->BindAction(IA_Action, ETriggerEvent::Completed, this, &AAJH_Player::OnMyAction);
		input->BindAction(IA_Tap, ETriggerEvent::Started, this, &AAJH_Player::OnMyActionTap);
	}

}

void AAJH_Player::OnMyActionMove(const FInputActionValue& value)
{
	FVector2D MovementVector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = this->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AAJH_Player::OnMyActionInteration(const FInputActionValue& value)
{
	InteractionLineTraceFuntion();
	if (bHit && outHit.GetActor()->ActorHasTag(TEXT("FarmTile")))
	{
		outHit.GetActor()->GetName();
		FString objectName = outHit.GetActor()->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, objectName);
	}
}

void AAJH_Player::OnMyAction(const FInputActionValue& value)
{
	InteractionLineTraceFuntion();
	if (bHit && outHit.GetActor()->ActorHasTag(TEXT("FarmTile")))
	{
		outHit.GetActor()->GetName();
		FString objectName = outHit.GetActor()->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, objectName);
	}
}

void AAJH_Player::OnMyActionTap()
{
	
}

void AAJH_Player::MouseCusorEvent()
{
	InteractionLineTraceFuntion();
	if (bHit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Hit!!!"), bHit);

	}
}

void AAJH_Player::InteractionLineTraceFuntion()
{
	// 스크린에 있는 마우스의 정보를 가져오는 함수
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectMousePositionToWorld(worldLoc, worldDir);
	// 라인 트레이스 출력
	start = worldLoc;
	// 엔드 지점
	end = start + worldDir * 2000;
	// 방어코드
	param;
	param.AddIgnoredActor(this);
	bHit = GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility, param);
}

void AAJH_Player::OnMyBoxCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	farmTile = Cast<AAJH_FarmTile>(OtherActor);
	treeTile = Cast<AJS_Tree>(OtherActor);
	if (farmTile && OtherActor->ActorHasTag(TEXT("FarmTile")))
	{
		farmTile->boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		farmTile->bodyMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		
		OtherActor->GetName();
		FString objectName = OtherActor->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, objectName);
	}
}

void AAJH_Player::OnMyBoxCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	farmTile = Cast<AAJH_FarmTile>(OtherActor);
	treeTile = Cast<AJS_Tree>(OtherActor);
	if (farmTile && OtherActor->ActorHasTag(TEXT("FarmTile")))
	{
		farmTile->boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		farmTile->bodyMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	}
}

void AAJH_Player::ReqTodayWeather(FString url, FString json)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	// 요청할 정보를 설정
	req->SetURL(url);
	req->SetVerb("POST");
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(json);

	// 응답 받을 함수를 연결
	req->OnProcessRequestComplete().BindUObject(this, &AAJH_Player::OnResTodayWeather);
	// 서버에 요청
	req->ProcessRequest();
	UE_LOG(LogTemp, Warning, TEXT("Request Sent"));
}

void AAJH_Player::OnResTodayWeather(FHttpRequestPtr HttpReq, FHttpResponsePtr HttpRes, bool bConnectedSuccessfully)
{
	UE_LOG(LogTemp, Warning, TEXT("OnResTodayWeather Start"));
	if (bConnectedSuccessfully)
	{
		// 성공
		FString result = HttpRes->GetContentAsString();
		UE_LOG(LogTemp, Log, TEXT("Response: %s"), *result);

		// Json 파싱 준비
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(result);

		// Json 파싱
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			// 오늘 날씨 정보를 추출
			TSharedPtr<FJsonObject> TodayWeather = JsonObject->GetObjectField("today");
			float TodayTemperature = TodayWeather->GetNumberField("temperature");
			FString TodayWeatherDesc = TodayWeather->GetStringField("weather");
			int32 TodayHumidity = TodayWeather->GetIntegerField("humidity");
			float TodayWindSpeed = TodayWeather->GetNumberField("wind_speed");

			UE_LOG(LogTemp, Log, TEXT("Today's Weather:"));
			UE_LOG(LogTemp, Log, TEXT("Temperature: %.2f"), TodayTemperature);
			UE_LOG(LogTemp, Log, TEXT("Weather: %s"), *TodayWeatherDesc);
			UE_LOG(LogTemp, Log, TEXT("Humidity: %d"), TodayHumidity);
			UE_LOG(LogTemp, Log, TEXT("Wind Speed: %.2f"), TodayWindSpeed);

			// 내일 날씨 정보를 추출 (필요에 따라)
			TSharedPtr<FJsonObject> TomorrowWeather = JsonObject->GetObjectField("tomorrow");
			float TomorrowTemperature = TomorrowWeather->GetNumberField("temperature");
			FString TomorrowWeatherDesc = TomorrowWeather->GetStringField("weather");
			int32 TomorrowHumidity = TomorrowWeather->GetIntegerField("humidity");
			float TomorrowWindSpeed = TomorrowWeather->GetNumberField("wind_speed");

			// UI에 표시할 텍스트를 구성
			FString DisplayText = FString::Printf(TEXT("Today's Weather:\nTemperature: %.2f\nWeather: %s\nHumidity: %d\nWind Speed: %.2f"),
				TodayTemperature, *TodayWeatherDesc, TodayHumidity, TodayWindSpeed);

			if (httpWeatherUI)
			{
				// 필요한 정보만 뽑아서 화면에 출력하고 싶다.
				httpWeatherUI->SetTextLog(result);
			}
		}
		else
		{
			// JSON 파싱 실패 시 에러 처리
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed to parse JSON response"));
		}
	}
	else
	{
		// 응답 실패 시 에러 처리
		UE_LOG(LogTemp, Error, TEXT("Failed to get weather data or connection unsuccessful"));
	}
}
