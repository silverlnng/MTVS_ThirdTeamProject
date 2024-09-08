// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_SecondRicePlant.h"
#include "Components/SphereComponent.h"
#include "JS_Rice.h"
#include "JS_Pumpkin.h"
#include "JS_Pumpkin.h"

// Sets default values
AJS_SecondRicePlant::AJS_SecondRicePlant()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	SetRootComponent(sphereComp);
	sphereComp->SetRelativeScale3D(FVector(1));
	sphereComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	sphereMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sphereMeshComp"));
	sphereMeshComp->SetupAttachment(sphereComp);
	sphereMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

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

	if (bInteractSecondPlant) {
		if (this->ActorHasTag(TEXT("Wheat")))
		{
			UE_LOG(LogTemp, Warning, TEXT("This actor has the Wheat tag."));
			SpawnNextPlant_Implementation(0);
			GetDamage_Implementation(true);
		}
		else if (this->ActorHasTag(TEXT("PumpKin"))) {
			 UE_LOG(LogTemp, Warning, TEXT("This actor has the PumpKin tag."));
			 SpawnNextPlant_Implementation(1);
			 GetDamage_Implementation(true);
		}
		else if (this->ActorHasTag(TEXT("Carrot"))) {
			 UE_LOG(LogTemp, Warning, TEXT("This actor has the Carrot tag."));
			 SpawnNextPlant_Implementation(2);
			 GetDamage_Implementation(true);
		}
	}
}

void AJS_SecondRicePlant::GetDamage_Implementation(bool damage)
{
	if (damage) SetCurHP_Implementation(curHP - 1);
}

void AJS_SecondRicePlant::SetCurHP_Implementation(float amount)
{
	if (amount <= 0) {
		Death_Implementation();
	}
	else curHP = amount;
}

void AJS_SecondRicePlant::SpawnNextPlant_Implementation(int32 index)
{
	switch (index)
	{
	case 0:
		PlantClassToSpawn = RiceFactory;
		UE_LOG(LogTemp, Warning, TEXT("id : 0 RiceFactory spawn"));
		break;
	case 1:
		PlantClassToSpawn = PumpkinFactory;
		UE_LOG(LogTemp, Warning, TEXT("id : 1 PumpkinFactory spawn"));
		break;
	case 2:
		PlantClassToSpawn = CarrotFactory;
		UE_LOG(LogTemp, Warning, TEXT("id : 2 CarrotFactory spawn"));
		break;
	default:
		break;
	}

	if (PlantClassToSpawn && this->ActorHasTag(TEXT("Wheat"))) {
		//선택된 블루프린트 클래스로 액터 스폰
		AJS_Rice* SpawnPlant = GetWorld()->SpawnActor<AJS_Rice>(PlantClassToSpawn, GetActorLocation(), FRotator::ZeroRotator);
		if (SpawnPlant) {
			SpawnPlant;
		}
		else UE_LOG(LogTemp, Warning, TEXT("Failed to spawn SpawnPlant"));
	}
	if (PlantClassToSpawn && this->ActorHasTag(TEXT("Pumpkin"))) {
		//선택된 블루프린트 클래스로 액터 스폰
		AJS_Pumpkin* SpawnPlant = GetWorld()->SpawnActor<AJS_Pumpkin>(PlantClassToSpawn, GetActorLocation(), FRotator::ZeroRotator);
		if (SpawnPlant) {
			SpawnPlant;
		}
		else UE_LOG(LogTemp, Warning, TEXT("Failed to spawn SpawnPlant"));
	}
	if (PlantClassToSpawn && this->ActorHasTag(TEXT("Carrot"))) {
		//선택된 블루프린트 클래스로 액터 스폰
		AJS_Carrot* SpawnPlant = GetWorld()->SpawnActor<AJS_Carrot>(PlantClassToSpawn, GetActorLocation(), FRotator::ZeroRotator);
		if (SpawnPlant) {
			SpawnPlant;
		}
		else UE_LOG(LogTemp, Warning, TEXT("Failed to spawn SpawnPlant"));
	}
}

void AJS_SecondRicePlant::Death_Implementation()
{
	this->Destroy();
}

