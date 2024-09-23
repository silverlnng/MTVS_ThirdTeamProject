// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_Well.h"
#include "Components/BoxComponent.h"

// Sets default values
AJS_Well::AJS_Well()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	staticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("framEquipment"));
	staticMeshComp->SetupAttachment(boxComp);
	staticMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	bReplicates = true;
}

// Called when the game starts or when spawned
void AJS_Well::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJS_Well::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}