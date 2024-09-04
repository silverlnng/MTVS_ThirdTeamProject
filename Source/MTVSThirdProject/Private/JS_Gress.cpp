// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_Gress.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AJS_Gress::AJS_Gress()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(RootComponent);

	gressMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gressMeshComp"));
	gressMeshComp->SetupAttachment(boxComp);
	gressMeshComp->SetRelativeScale3D(FVector(0.5f));

	ConstructorHelpers::FObjectFinder <UStaticMesh> gressMeshTemp(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (gressMeshTemp.Succeeded()) {
		gressMeshComp->SetStaticMesh(gressMeshTemp.Object);
	}
}

// Called when the game starts or when spawned
void AJS_Gress::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJS_Gress::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJS_Gress::GetDamage_Implementation(bool damage)
{
	if (damage) SetCurHP_Implementation(curHP - 1);
}

void AJS_Gress::SetCurHP_Implementation(float amount)
{
	if (amount <= 0) {
		Death_Implementation();
	}
	else curHP = amount;
}

void AJS_Gress::Death_Implementation()
{
	this->Destroy();
}

