// Fill out your copyright notice in the Description page of Project Settings.


#include "AJH_Player.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AAJH_Player::AAJH_Player()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	springArmComp->SetupAttachment(RootComponent);
	springArmComp->TargetArmLength = 800;

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
	
	/*APawn* ControlledPawn = GetWorld()->GetPawn();
	if (ControlledPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn is correctly controlled."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Pawn is not controlled."));
	}*/
}

// Called every frame
void AAJH_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAJH_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	

}



