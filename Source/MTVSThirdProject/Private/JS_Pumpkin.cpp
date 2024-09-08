// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_Pumpkin.h"
#include "Components/SphereComponent.h"

// Sets default values
AJS_Pumpkin::AJS_Pumpkin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	SetRootComponent(sphereComp);
	sphereComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);
	sphereComp->SetRelativeScale3D(FVector(1));
	sphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	pumpkinMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("pumpkinMeshComp"));
	pumpkinMeshComp->SetupAttachment(sphereComp);
	pumpkinMeshComp->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	pumpkinMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	ConstructorHelpers::FObjectFinder<UStaticMesh> sphereMeshTemp(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (sphereMeshTemp.Succeeded()) {
		pumpkinMeshComp->SetStaticMesh(sphereMeshTemp.Object);
	}
}

// Called when the game starts or when spawned
void AJS_Pumpkin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJS_Pumpkin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

