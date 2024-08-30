// Fill out your copyright notice in the Description page of Project Settings.


#include "AJH_Player.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

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
	
	
}

// Called every frame
void AAJH_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MouseCusorEvent();
	PlayerInteractionSize();
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

void AAJH_Player::MouseCusorEvent()
{
	// ���콺�� ��ġ
	FVector worldLoc;
	// ���콺�� ����
	FVector worldDir;
	// ��ũ���� �ִ� ���콺�� ������ �������� �Լ�
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->DeprojectMousePositionToWorld(worldLoc, worldDir);
	// ���� Ʈ���̽� ���
	FHitResult outHit;
	// ��ŸƮ ����
	FVector start = worldLoc;
	// ���� ����
	FVector end = start + worldDir * 2000;
	// ����ڵ�
	FCollisionQueryParams param;
	param.AddIgnoredActor(this);
	bool bHit = GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility, param);
	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit!!!"), bHit);
		FString objectName = outHit.GetActor()->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, objectName);
	}
	else
	{
		//DrawDebugLine(GetWorld(), start, end, FColor::Red, false, -1, 1, 2);
	}

}

void AAJH_Player::PlayerInteractionSize()
{
	// ��ŸƮ����
	FVector myDrawLine = this->GetActorLocation();
	for (int i = -2; i < tileX + 1; i++)
	{
		// LineStart : ������������ X ������ i �� ������ ��ŭ �̵��Ͽ� ���� �ϳ� ��´�
		// LineEnd : ������������ X ������ i�� ������ ��ŭ �̵� �� Y ������ tileY  * ������ ��ŭ�� ���� ��´�
		// FColor : ���� �̾ ������ �׷��ش�
		DrawDebugLine(GetWorld(), myDrawLine + FVector::ForwardVector * i * 100, myDrawLine + FVector::ForwardVector * i * 100 + FVector::RightVector * tileY * 100, FColor::Red, false, -1, 1, 2);

	}
	for (int i = -2; i < tileY + 1; i++)
	{
		DrawDebugLine(GetWorld(), myDrawLine + FVector::RightVector * i * 100, myDrawLine + FVector::RightVector * i * 100 + FVector::ForwardVector * tileX * 100, FColor::Red, false, -1, 1, 2);
	}
}

