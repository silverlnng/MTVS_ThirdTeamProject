// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_Warp.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

// Sets default values
AJS_Warp::AJS_Warp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	boxComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);


	staticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	staticMeshComp->SetupAttachment(boxComp);
	staticMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void AJS_Warp::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AJS_Warp::OnOverlapBegin);
}

// Called every frame
void AJS_Warp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	checkTime += DeltaTime;
}

void AJS_Warp::WarpPlayer(ACharacter* Player)
{
	if (targetWarpPoint && Player && checkTime >= waitTime){
		checkTime = 0;
		FVector TargetLocation = targetWarpPoint->GetActorLocation();
		Player->SetActorLocation(TargetLocation);
	}
}

void AJS_Warp::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Player = Cast<ACharacter>(OtherActor);
	if (Player)
	{
		WarpPlayer(Player);
	}
}

