﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_Watermelon.h"
#include "Components/SphereComponent.h"
#include "AJH_Player.h"

// Sets default values
AJS_Watermelon::AJS_Watermelon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	SetRootComponent(sphereComp);
	sphereComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);
	sphereComp->SetRelativeScale3D(FVector(1));
	sphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	carrotMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("carrotMeshComp"));
	carrotMeshComp->SetupAttachment(sphereComp);
	carrotMeshComp->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	carrotMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AJS_Watermelon::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if ( playerController ) {
		SetOwner(playerController);
	}
	sphereComp->OnComponentBeginOverlap.AddDynamic(this , &AJS_Watermelon::OnOverlapBegin);
}

// Called every frame
void AJS_Watermelon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJS_Watermelon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	Server_OnOverlapBegin(OtherActor , true);
}

void AJS_Watermelon::Server_OnOverlapBegin_Implementation(AActor* OtherActor , bool bOverlapBegin)
{
	if ( !OtherActor ) {
		UE_LOG(LogTemp , Warning , TEXT("Strawberry Actor ServerBeginOverlap : null"));
		return;
	}

	if ( bOverlapBegin && OtherActor->IsA(AAJH_Player::StaticClass()) ) {
		UE_LOG(LogTemp , Warning , TEXT("Strawberry Actor Overlap With Player"));
		Multicast_OnOverlapBegin(OtherActor , true);
	}
}

void AJS_Watermelon::Multicast_OnOverlapBegin_Implementation(AActor* OtherActor , bool bOverlapBegin)
{
	if ( !OtherActor ) {
		UE_LOG(LogTemp , Warning , TEXT("Strawberry Actor MultiBeginOverlap : null"));
		return;
	}
	if ( bOverlapBegin ) {
		//플레이어에 값을 만들어서 삭제되면 값이 1증가 하도록 설정
		UE_LOG(LogTemp , Warning , TEXT("Strawberry Actor MultiOverlap In"));
		this->Destroy();
	}
}
