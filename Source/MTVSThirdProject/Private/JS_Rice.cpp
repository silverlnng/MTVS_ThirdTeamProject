// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_Rice.h"
#include "Components/SphereComponent.h"

// Sets default values
AJS_Rice::AJS_Rice()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	SetRootComponent(sphereComp);
	sphereComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);
	sphereComp->SetRelativeScale3D(FVector(1));
	sphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	riceMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("riceMeshComp"));
	riceMeshComp->SetupAttachment(sphereComp);
	riceMeshComp->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	riceMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	ConstructorHelpers::FObjectFinder<UStaticMesh> sphereMeshTemp(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (sphereMeshTemp.Succeeded()) {
		riceMeshComp->SetStaticMesh(sphereMeshTemp.Object);
	}
}

// Called when the game starts or when spawned
void AJS_Rice::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJS_Rice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

