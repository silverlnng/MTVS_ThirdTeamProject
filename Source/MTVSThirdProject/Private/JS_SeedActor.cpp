// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_SeedActor.h"
#include "JS_FirstRicePlant.h"

// Sets default values
AJS_SeedActor::AJS_SeedActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	if (bSeedActorInteract && checkDeltaTime >= growTime) {
		//상호작용은 한 번만
		bSeedActorInteract = false;
		//씨를 파괴하고 벼 작물로 스폰
		SpawnNextPlant_Implementation(0);
		GetDamage_Implementation(true);
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
	
	if (!SpawnFirstRicePlnat) SpawnFirstRicePlnat = AJS_FirstRicePlant::StaticClass();
	if (SpawnFirstRicePlnat) {
		AJS_FirstRicePlant* firstRicePlant = GetWorld()->SpawnActor<AJS_FirstRicePlant>(SpawnFirstRicePlnat, GetActorLocation(), FRotator::ZeroRotator);
		if (firstRicePlant) {
			switch (index) {
			case 0:
				firstRicePlant;
				break;
			default:
				break;
			}
		}
		else UE_LOG(LogTemp, Warning, TEXT("Failed to spawn AJS_FirstRicePlant"));
	}
	else UE_LOG(LogTemp, Warning, TEXT("SpawnFirstRicePlnat is not set!"));
	
}

void AJS_SeedActor::Death_Implementation()
{
	this->Destroy();
}

