// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_SeedActor.h"
#include "JS_FirstRicePlant.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AJS_SeedActor::AJS_SeedActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50));
	boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	seedMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("seedMeshComp"));
	seedMeshComp->SetupAttachment(boxComp);
	seedMeshComp->SetRelativeScale3D(FVector(1, 1, 0.1f));
	seedMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void AJS_SeedActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJS_SeedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	checkDeltaTime += DeltaTime;

	if (checkDeltaTime >= growTime) {
		//상호작용은 한 번만
		if (this->ActorHasTag(TEXT("Wheat"))) {
			//씨를 파괴하고 벼 작물로 스폰
			UE_LOG(LogTemp, Warning, TEXT("This actor has the Wheat tag."));
			SpawnNextPlant_Implementation(0);
			GetDamage_Implementation(true);
		}
		if(this->ActorHasTag(TEXT("PumpKin"))) {
			UE_LOG(LogTemp, Warning, TEXT("This actor has the PumpKin tag."));
			SpawnNextPlant_Implementation(1);
			GetDamage_Implementation(true);
		}
		if (this->ActorHasTag(TEXT("Carrot"))) {
			UE_LOG(LogTemp, Warning, TEXT("This actor has the Carrot tag."));
			SpawnNextPlant_Implementation(2);
			GetDamage_Implementation(true);
		}	
	}
}

void AJS_SeedActor::GetDamage_Implementation(bool damage)
{
	if(damage) SetCurHP_Implementation(curHP - 1);
}

void AJS_SeedActor::SetCurHP_Implementation(float amount)
{
	if (amount <= 0) {
		Death_Implementation();
	}
	else curHP = amount;
}

void AJS_SeedActor::SpawnNextPlant_Implementation(int32 index)
{
	/*if (!SpawnFirstRicePlnat) SpawnFirstRicePlnat = AJS_FirstRicePlant::StaticClass();*/
	switch (index)
	{
	case 0 :
		PlantClassToSpawn = SpawnFirstRicePlant;
		UE_LOG(LogTemp, Warning, TEXT("id : 0 firstRicePlant spawn"));
		break;
	case 1 :
		PlantClassToSpawn = SpawnFirstPumpkinPlant;
		UE_LOG(LogTemp, Warning, TEXT("id : 1 firstRicePlant spawn"));
		break;
	case 2 :
		PlantClassToSpawn = SpawnFirstCarrotPlant;
		UE_LOG(LogTemp, Warning, TEXT("id : 2 firstRicePlant spawn"));
		break;
	default:
		break;
	}

	if (PlantClassToSpawn) {
		//선택된 블루프린트 클래스로 액터 스폰
		AJS_FirstRicePlant* SpawnPlant = GetWorld()->SpawnActor<AJS_FirstRicePlant>(PlantClassToSpawn, GetActorLocation(), FRotator::ZeroRotator);
		if (SpawnPlant) {
			SpawnPlant;
		}
		else UE_LOG(LogTemp, Warning, TEXT("Failed to spawn AJS_FirstRicePlant"));
	}
	
}

void AJS_SeedActor::Death_Implementation()
{
	this->Destroy();
}

