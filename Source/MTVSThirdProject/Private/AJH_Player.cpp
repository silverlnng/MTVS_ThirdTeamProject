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
#include "AJH_JsonParseLib.h"
#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "MTVSThirdProject/YJ/NetWorkGameInstance.h"
#include "MTVSThirdProject/YJ/UserNameWidget.h"


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

	UserNameWidgetComp =CreateDefaultSubobject<UWidgetComponent>(TEXT("UserNameWidget"));
	UserNameWidgetComp->SetupAttachment(GetMesh());
	UserNameWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

	ConstructorHelpers::FClassFinder<UUserNameWidget> tempUserNameWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/YJ/UI/WBP_UserName.WBP_UserName_C'"));

	if (tempUserNameWidget.Succeeded())
	{
		UserNameWidgetComp->SetWidgetClass(tempUserNameWidget.Class);
	}
	
}

// Called when the game starts or when spawned
void AAJH_Player::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AAJH_Player::OnMyBoxCompBeginOverlap);
	boxComp->OnComponentEndOverlap.AddDynamic(this, &AAJH_Player::OnMyBoxCompEndOverlap);
	httpWeatherUI = Cast<UAJH_WeatherWidget>(UGameplayStatics::GetActorOfClass(GetWorld(), UAJH_WeatherWidget::StaticClass()));
	
	
	UserNameUI = Cast<UUserNameWidget>(UserNameWidgetComp->GetWidget());

	gi =GetGameInstance<UNetWorkGameInstance>();

	//로컬플레이어만 ServerChange 실행
	if(GetController() && GetController()->IsLocalController())
	{
		ServerChange(gi->UserNickName,gi->meshNum);
	}
	// 모든플레이어가 change실행
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle,[this]()
	{
		UserNameUI->SetUserName(UserName);
	},1.f,false);
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
	httpWeatherUI = Cast<UAJH_WeatherWidget>(CreateWidget(GetWorld(), weatherUI));
	if (httpWeatherUI)
	{
		httpWeatherUI->AddToViewport();
		httpWeatherUI->SetPlayerHttp(this);
	}
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
	// ��ũ���� �ִ� ���콺�� ������ �������� �Լ�
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectMousePositionToWorld(worldLoc, worldDir);
	// ���� Ʈ���̽� ���
	start = worldLoc;
	// ���� ����
	end = start + worldDir * 2000;
	// ����ڵ�
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
		
		// �ݸ��� ä�� ���� �� Ȯ��
		ECollisionResponse Response = farmTile->boxComp->GetCollisionResponseToChannel(ECC_Visibility);
		UE_LOG(LogTemp, Warning, TEXT("Collision response set to: %d"), Response);
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

void AAJH_Player::ReqTodayWeather(FString url)
{
	//FHttpModule& httpModule = FHttpModule::Get();
	//TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	//// ��û�� ������ ����
	//req->SetURL(url);
	//req->SetVerb("GET");
	//req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	//// req->SetContentAsString(json);

	//// ���� ���� �Լ��� ����
	//req->OnProcessRequestComplete().BindUObject(this, &AAJH_Player::OnResTodayWeather);
	//// ������ ��û
	//req->ProcessRequest();
	//UE_LOG(LogTemp, Warning, TEXT("Request Sent : %s"), *url);
	// ��û�� ������ ����
	/*req->SetURL(url);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(json);*/

	// ���� ���� �Լ��� ����
	//req->OnProcessRequestComplete().BindUObject(this, &AAJH_Player::OnResTodayWeather);
	// ������ ��û
	//req->ProcessRequest();
}

void AAJH_Player::OnResTodayWeather(FHttpRequestPtr HttpReq, FHttpResponsePtr HttpRes, bool bConnectedSuccessfully)
{
	//if (bConnectedSuccessfully)
	//{
	//	// ����
	//	FString result = HttpRes->GetContentAsString();

	//	httpWeatherUI->SetTextLog(UAJH_JsonParseLib::WeatherJsonParse(result));
	//	UE_LOG(LogTemp, Warning, TEXT("result : %s"), *result);
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("OnResTodayWeather Faild..."));
	//}
}

//void AAJH_Player::OnResTodayWeather(FHttpRequestPtr HttpReq, FHttpResponsePtr HttpRes, bool bConnectedSuccessfully)
//{
//	UE_LOG(LogTemp, Warning, TEXT("OnResTodayWeather Start"));
//	if (bConnectedSuccessfully)
//	{
//		// ����
//		FString result = HttpRes->GetContentAsString();
//		UE_LOG(LogTemp, Log, TEXT("Response: %s"), *result);
//
//		// Json �Ľ� �غ�
//		TSharedPtr<FJsonObject> JsonObject;
//		TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(result);
//
//		// Json �Ľ�
//		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
//		{
//			// ���� ���� ������ ����
//			TSharedPtr<FJsonObject> TodayWeather = JsonObject->GetObjectField("today");
//			float TodayTemperature = TodayWeather->GetNumberField("temperature");
//			FString TodayWeatherDesc = TodayWeather->GetStringField("weather");
//			int32 TodayHumidity = TodayWeather->GetIntegerField("humidity");
//			float TodayWindSpeed = TodayWeather->GetNumberField("wind_speed");
//
//			UE_LOG(LogTemp, Log, TEXT("Today's Weather:"));
//			UE_LOG(LogTemp, Log, TEXT("Temperature: %.2f"), TodayTemperature);
//			UE_LOG(LogTemp, Log, TEXT("Weather: %s"), *TodayWeatherDesc);
//			UE_LOG(LogTemp, Log, TEXT("Humidity: %d"), TodayHumidity);
//			UE_LOG(LogTemp, Log, TEXT("Wind Speed: %.2f"), TodayWindSpeed);
//
//			// ���� ���� ������ ���� (�ʿ信 ����)
//			TSharedPtr<FJsonObject> TomorrowWeather = JsonObject->GetObjectField("tomorrow");
//			float TomorrowTemperature = TomorrowWeather->GetNumberField("temperature");
//			FString TomorrowWeatherDesc = TomorrowWeather->GetStringField("weather");
//			int32 TomorrowHumidity = TomorrowWeather->GetIntegerField("humidity");
//			float TomorrowWindSpeed = TomorrowWeather->GetNumberField("wind_speed");
//
//			// UI�� ǥ���� �ؽ�Ʈ�� ����
//			FString DisplayText = FString::Printf(TEXT("Today's Weather:\nTemperature: %.2f\nWeather: %s\nHumidity: %d\nWind Speed: %.2f"),
//				TodayTemperature, *TodayWeatherDesc, TodayHumidity, TodayWindSpeed);
//
//			if (httpWeatherUI)
//			{
//				// �ʿ��� ������ �̾Ƽ� ȭ�鿡 ����ϰ� �ʹ�.
//				httpWeatherUI->SetTextLog(result);
//			}
//		}
//		else
//		{
//			// JSON �Ľ� ���� �� ���� ó��
//			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Failed to parse JSON response"));
//		}
//	}
//	else
//	{
//		// ���� ���� �� ���� ó��
//		UE_LOG(LogTemp, Error, TEXT("Failed to get weather data or connection unsuccessful"));
//	}
//}
	/*if (bConnectedSuccessfully)
	{
		// ����
		FString result = Response->GetContentAsString();
		// �ʿ��� ������ �̾Ƽ� ȭ�鿡 ����ϰ� �ʹ�.
		httpWeatherUI->SetTextLog(result);
	}*/


void AAJH_Player::ServerChange_Implementation(const FString& userName_, int32 meshNum_)
{
	MeshNum = meshNum_;
	UserName = userName_;
}

void AAJH_Player::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAJH_Player,UserName);
	DOREPLIFETIME(AAJH_Player,MeshNum);
}
