// Fill out your copyright notice in the Description page of Project Settings.


#include "AJH_Player.h"
#include "AJH_FarmTile.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "HttpModule.h"
#include "AJH_WeatherWidget.h"
#include "AJH_JsonParseLib.h"
#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "MTVSThirdProject/YJ/NetWorkGameInstance.h"
#include "MTVSThirdProject/YJ/UserNameWidget.h"
#include "JS_Rice.h"
#include "JS_Pumpkin.h"
#include "JS_Carrot.h"
#include "JS_SeedActor.h"
#include "JS_ObstacleActor.h"
#include "AJH_PlayerAnimInstance.h"
#include "MTVSThirdProject/YJ/HttpWidget.h"


// Sets default values
AAJH_Player::AAJH_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->SetRelativeRotation(FRotator(-55, 0, 0));
	springArmComp->TargetArmLength = 650;
	springArmComp->bUsePawnControlRotation = false;
	// 카메라의 회전을 고정시키기 위한 설정
	springArmComp->bInheritPitch = false;
	springArmComp->bInheritYaw = false;
	springArmComp->bInheritRoll = false;

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	cameraComp->SetupAttachment(springArmComp, USpringArmComponent::SocketName);
	cameraComp->bUsePawnControlRotation = false;

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
	object = Cast<AJS_ObstacleActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AJS_ObstacleActor::StaticClass()));
	anim = CastChecked<UAJH_PlayerAnimInstance>(GetMesh()->GetAnimInstance());
	
	UserNameUI = Cast<UUserNameWidget>(UserNameWidgetComp->GetWidget());

	if(GetController() && GetController()->IsLocalController())
	{
		HttpUI = CreateWidget<UHttpWidget>(GetWorld(), HttpUI_factory);
		if(HttpUI)
		{
			HttpUI->AddToViewport();
		}
	}
	
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

	//MouseCusorEvent();

	
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
		input->BindAction(IA_Tap, ETriggerEvent::Started, this, &AAJH_Player::OnMyActionTap);
		input->BindAction(IA_SelectNone, ETriggerEvent::Started, this, &AAJH_Player::OnMySelectedNone);
		input->BindAction(IA_SelectRiceSeed, ETriggerEvent::Started, this, &AAJH_Player::OnMySelectRiceSeed);
		input->BindAction(IA_SelectPumpkinSeed, ETriggerEvent::Started, this, &AAJH_Player::OnMySelectPumpkinSeed);
		input->BindAction(IA_SelectCarrotSeed, ETriggerEvent::Started, this, &AAJH_Player::OnMySelectCarrotSeed);
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

	if (outHit.GetComponent()->GetCollisionResponseToChannel(ECC_Visibility) == ECR_Block)
	{
		// Tag : Tree, Rock, Gress
		if (bHit && outHit.GetActor()->ActorHasTag(TEXT("Tree")) && selectedSeedType == ESeedType::None && anim && anim->bAttackAnimation == false)
		{
			// 몽타주 재생
			anim->PlayMeleeAttackMontage();

			object = Cast<AJS_ObstacleActor>(outHit.GetActor());
			object->GetDamage_Implementation(true);
			// hp 체크용
			int32 hp = object->curHP;
			UE_LOG(LogTemp, Warning, TEXT("hp : %d"), hp);
		}
		else if (bHit && outHit.GetActor()->ActorHasTag(TEXT("Rock")) && selectedSeedType == ESeedType::None && anim && anim->bAttackAnimation == false)
		{
			// 몽타주 재생
			anim->PlayMeleeAttackMontage();

			object = Cast<AJS_ObstacleActor>(outHit.GetActor());
			object->GetDamage_Implementation(true);
			int32 hp = object->curHP;
			UE_LOG(LogTemp, Warning, TEXT("hp : %d"), hp);
		}
		else if (bHit && outHit.GetActor()->ActorHasTag(TEXT("Gress")) && selectedSeedType == ESeedType::None && anim && anim->bAttackAnimation == false)
		{
			// 몽타주 재생
			anim->PlayMeleeAttackMontage();

			object = Cast<AJS_ObstacleActor>(outHit.GetActor());
			object->GetDamage_Implementation(true);
			int32 hp = object->curHP;
			UE_LOG(LogTemp, Warning, TEXT("hp : %d"), hp);
		}
		else if (bHit && outHit.GetActor()->ActorHasTag(TEXT("LandTile")))
		{
			switch (selectedSeedType)
			{
			case ESeedType::None:
				ActionNone();
				break;
			case ESeedType::RiceSeed:
				ActionRice();
				break;
			case ESeedType::PumpkinSeed:
				ActionPumpkin();
				break;
			case ESeedType::CarrotSeed:
				ActionCarrot();
				break;
			default:
				break;
			}
		}
		else if (bHit && outHit.GetActor()->ActorHasTag(TEXT("Seed")))
		{
			outHit.GetActor()->GetName();
			FString objectName = outHit.GetActor()->GetName();
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, objectName);
		}
	}

}

void AAJH_Player::OnMyActionTap()
{
	/*httpWeatherUI = Cast<UAJH_WeatherWidget>(CreateWidget(GetWorld(), weatherUI));
	if (httpWeatherUI)
	{
		httpWeatherUI->AddToViewport();
		httpWeatherUI->SetPlayerHttp(this);
	}*/
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
	// 
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectMousePositionToWorld(worldLoc, worldDir);
	// 
	start = worldLoc;
	// 
	end = start + worldDir * 2000;
	// 
	param;
	param.AddIgnoredActor(this);
	bHit = GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility, param);
}

void AAJH_Player::OnMySelectedSeed(ESeedType newSeedType)
{
	selectedSeedType = newSeedType;
}

void AAJH_Player::OnMySelectedNone()
{
	OnMySelectedSeed(ESeedType::None);
	UE_LOG(LogTemp, Warning, TEXT("Selected Seed : None"));
}

void AAJH_Player::OnMySelectRiceSeed()
{
	OnMySelectedSeed(ESeedType::RiceSeed);
	UE_LOG(LogTemp, Warning, TEXT("Selected Seed : Rice"));
}

void AAJH_Player::OnMySelectPumpkinSeed()
{
	OnMySelectedSeed(ESeedType::PumpkinSeed);
	UE_LOG(LogTemp, Warning, TEXT("Selected Seed : Pumpkin"));
}

void AAJH_Player::OnMySelectCarrotSeed()
{
	OnMySelectedSeed(ESeedType::CarrotSeed);
	UE_LOG(LogTemp, Warning, TEXT("Selected Seed : Carrrot"));
}

void AAJH_Player::ActionNone()
{
	
	UE_LOG(LogTemp, Warning, TEXT("Action : None"));
}

void AAJH_Player::ActionRice()
{
	seedParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	GetWorld()->SpawnActor<AJS_SeedActor>(SeedFatory, outHit.GetActor()->GetActorTransform(), seedParam);
	// seed->SpawnNextPlant_Implementation(0);
	UE_LOG(LogTemp, Warning, TEXT("Action : Rice"));
}

void AAJH_Player::ActionPumpkin()
{
	seedParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	GetWorld()->SpawnActor<AJS_SeedActor>(SeedFatory, outHit.GetActor()->GetActorTransform(), seedParam);
	// seed->SpawnNextPlant_Implementation(1);
	UE_LOG(LogTemp, Warning, TEXT("Action : Pumpkin"));
}

void AAJH_Player::ActionCarrot()
{
	seedParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	GetWorld()->SpawnActor<AJS_SeedActor>(SeedFatory, outHit.GetActor()->GetActorTransform(), seedParam);
	// seed->SpawnNextPlant_Implementation(2);
	UE_LOG(LogTemp, Warning, TEXT("Action : Carrot"));
}

void AAJH_Player::OnMyBoxCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	farmTile = Cast<AAJH_FarmTile>(OtherActor);
	object = Cast<AJS_ObstacleActor>(OtherActor);
	if (HasAuthority())
	{
		ServerBeginOverlap(OtherActor, true);
	}
	else
	{
		ServerBeginOverlap(OtherActor, true);
	}
}

void AAJH_Player::OnMyBoxCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	farmTile = Cast<AAJH_FarmTile>(OtherActor);
	object = Cast<AJS_ObstacleActor>(OtherActor);
	if (HasAuthority())
	{
		ServerEndOverlap(OtherActor, true);
	}
	else
	{
		ServerEndOverlap(OtherActor, true);
	}
}

void AAJH_Player::ServerBeginOverlap_Implementation(AActor* OtherActor, bool bIsBeginOverlap)
{
	if (!OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerBeginOverlap : null"));
		return;
	}

	if (bIsBeginOverlap)
	{
		MulticastBeginOverlap(OtherActor ,bIsBeginOverlap);
		UE_LOG(LogTemp, Warning, TEXT("ServerBeginOverlap : Hello"));
	}
}

void AAJH_Player::MulticastBeginOverlap_Implementation(AActor* OtherActor, bool bIsBeginOverlap)
{
	if (!OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("MultiBeginOverlap : null"));
		return;
	}

	if (bIsBeginOverlap)
	{
		if (farmTile && OtherActor->ActorHasTag(TEXT("Seed")))
		{
			farmTile->boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
			farmTile->bodyMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

			ECollisionResponse Response = farmTile->boxComp->GetCollisionResponseToChannel(ECC_Visibility);
			UE_LOG(LogTemp, Warning, TEXT("Collision response set to: %d"), Response);
			OtherActor->GetName();
			FString objectName = OtherActor->GetName();
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, objectName);
		}
		if (object && OtherActor->ActorHasTag(TEXT("Tree")))
		{
			if (object->boxComp)
			{
				object->boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
			}
			if (object->staticMeshComp)
			{
				object->staticMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
			}
		}
		if (object && OtherActor->ActorHasTag(TEXT("Rock")))
		{
			if(object->boxComp)
			object->boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
			if(object->staticMeshComp)
			object->staticMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		}
		if (object && OtherActor->ActorHasTag(TEXT("Gress")))
		{
			if (object->boxComp)
			object->boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
			if(object->staticMeshComp)
			object->staticMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		}
	}

	
}

void AAJH_Player::ServerEndOverlap_Implementation(AActor* OtherActor, bool bIsBeginOverlap)
{
	if (!OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerBeginOverlap : null"));
		return;
	}

	if (bIsBeginOverlap)
	{
		MulticastEndOverlap(OtherActor ,bIsBeginOverlap);
		UE_LOG(LogTemp, Warning, TEXT("ServerEndOverlap : Bey"));
	}
}

void AAJH_Player::MulticastEndOverlap_Implementation(AActor* OtherActor, bool bIsBeginOverlap)
{
	if (!OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("MultiEndOverlap : null"));
		return;
	}
	if (bIsBeginOverlap)
	{
		UE_LOG(LogTemp, Warning, TEXT("MultiEndOverlap : Bye"));
		if (farmTile && OtherActor->ActorHasTag(TEXT("Seed")))
		{
			farmTile->boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			farmTile->bodyMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		}
		if (object && OtherActor->ActorHasTag(TEXT("Tree")))
		{
			object->boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			object->staticMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		}
		if (object && OtherActor->ActorHasTag(TEXT("Rock")))
		{
			object->boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			object->staticMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		}
		if (object && OtherActor->ActorHasTag(TEXT("Gress")))
		{
			object->boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
			object->staticMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
		}
	}

}

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
