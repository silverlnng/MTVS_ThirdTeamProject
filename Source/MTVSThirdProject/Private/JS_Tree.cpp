// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_Tree.h"
#include "Components/BoxComponent.h"

// Sets default values
AJS_Tree::AJS_Tree()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(RootComponent);

	treeMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TreeMeshComp"));
	treeMeshComp->SetupAttachment(boxComp);
	treeMeshComp->SetRelativeScale3D(FVector(1, 1, 2));

	ConstructorHelpers::FObjectFinder <UStaticMesh> treeMeshTemp(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (treeMeshTemp.Succeeded()) {
		treeMeshComp->SetStaticMesh(treeMeshTemp.Object);
	}
}

// Called when the game starts or when spawned
void AJS_Tree::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJS_Tree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJS_Tree::GetDamage_Implementation(bool damage)
{
	if (damage) SetCurHP_Implementation(curHP - 1);
}

void AJS_Tree::SetCurHP_Implementation(float amount)
{
	if (amount <= 0) {
		Death_Implementation();
	}
	else curHP = amount;
}

void AJS_Tree::SpawnNextPlant_Implementation(int32 index)
{
	
}

void AJS_Tree::Death_Implementation()
{
	this->Destroy();
}

