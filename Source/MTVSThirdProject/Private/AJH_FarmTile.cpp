// Fill out your copyright notice in the Description page of Project Settings.


#include "AJH_FarmTile.h"
#include "Components/BoxComponent.h"
#include "AJH_Player.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AAJH_FarmTile::AAJH_FarmTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50, 50, 50));

	bodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	bodyMesh->SetupAttachment(boxComp);

}

// Called when the game starts or when spawned
void AAJH_FarmTile::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
	
}

// Called every frame
void AAJH_FarmTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAJH_FarmTile::ServerChangeCollision_Implementation()
{

	MultiChangeCollision();

}

void AAJH_FarmTile::MultiChangeCollision_Implementation()
{
	boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	bodyMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
}

void AAJH_FarmTile::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAJH_FarmTile, boxComp);
	DOREPLIFETIME(AAJH_FarmTile, bodyMesh);

}

