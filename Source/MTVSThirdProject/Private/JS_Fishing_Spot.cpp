// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_Fishing_Spot.h"
#include "Components/BoxComponent.h"

// Sets default values
AJS_Fishing_Spot::AJS_Fishing_Spot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	boxComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Overlap);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AJS_Fishing_Spot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJS_Fishing_Spot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

