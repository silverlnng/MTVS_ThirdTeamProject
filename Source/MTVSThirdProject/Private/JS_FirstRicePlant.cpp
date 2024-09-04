// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_FirstRicePlant.h"
#include "Components/SphereComponent.h"
#include "JS_SecondRicePlant.h"

// Sets default values
AJS_FirstRicePlant::AJS_FirstRicePlant()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	SetRootComponent(sphereComp);

	sphereMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sphereMeshComp"));
	sphereMeshComp->SetupAttachment(sphereComp);

	ConstructorHelpers::FObjectFinder<UStaticMesh> sphereMeshTemp(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (sphereMeshTemp.Succeeded()) {
		sphereMeshComp->SetStaticMesh(sphereMeshTemp.Object);
	}
}

// Called when the game starts or when spawned
void AJS_FirstRicePlant::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJS_FirstRicePlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	checkDeltaTime += DeltaTime;
	if(checkDeltaTime >= growTime){
		SpawnNextPlant_Implementation(0);
		Death_Implementation();
	}
}

void AJS_FirstRicePlant::SpawnNextPlant_Implementation(int32 index)
{
	if(!SpawnSecondRicePlant) SpawnSecondRicePlant = AJS_SecondRicePlant::StaticClass();
	if (SpawnSecondRicePlant) {
		AJS_SecondRicePlant* secondRicePlant = GetWorld()->SpawnActor<AJS_SecondRicePlant>(SpawnSecondRicePlant, GetActorLocation(), FRotator::ZeroRotator);
		if (secondRicePlant) {
			secondRicePlant;
		}
	}
}

void AJS_FirstRicePlant::Death_Implementation()
{
	this->Destroy();
}

