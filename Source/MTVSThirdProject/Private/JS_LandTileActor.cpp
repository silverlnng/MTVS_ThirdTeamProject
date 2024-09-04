// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_LandTileActor.h"

// Sets default values
AJS_LandTileActor::AJS_LandTileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	landTileComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("landTileComp"));
	landTileComp->SetupAttachment(RootComponent);
	landTileComp->SetRelativeScale3D(FVector(1.f, 1.f, 0.1f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> landTileMeshTemp(TEXT("/ Script / Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	if (landTileMeshTemp.Succeeded()) {
		landTileComp->SetStaticMesh(landTileMeshTemp.Object);
	}
}

// Called when the game starts or when spawned
void AJS_LandTileActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJS_LandTileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//틱에서 레이를 맞았는지 계속 체크해서 
}

