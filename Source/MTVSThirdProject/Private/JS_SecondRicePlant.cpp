// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_SecondRicePlant.h"
#include "Components/SphereComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AJS_SecondRicePlant::AJS_SecondRicePlant()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	SetRootComponent(sphereComp);
	sphereComp->SetRelativeScale3D(FVector(2));

	sphereMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sphereMeshComp"));
	sphereMeshComp->SetupAttachment(sphereComp);
	

	ConstructorHelpers::FObjectFinder<UStaticMesh> sphereMeshTemp(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (sphereMeshTemp.Succeeded()) {
		sphereMeshComp->SetStaticMesh(sphereMeshTemp.Object);
	}
}

// Called when the game starts or when spawned
void AJS_SecondRicePlant::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJS_SecondRicePlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

