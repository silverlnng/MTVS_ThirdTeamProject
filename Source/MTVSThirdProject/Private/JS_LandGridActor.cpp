// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_LandGridActor.h"
#include "JS_LandTileActor.h"
#include "JS_Rock.h"
#include "JS_Tree.h"
#include "JS_Gress.h"

// Sets default values
AJS_LandGridActor::AJS_LandGridActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJS_LandGridActor::BeginPlay()
{
	Super::BeginPlay();

	for (int32 x = 0; x < gridSize; x++) {
		for (int32 y = 0; y < gridSize; y++) {
			FVector location = GetActorLocation() + FVector(x * landSpacing, y * landSpacing, 0.0f);

			if (TileFactory) {
				FActorSpawnParameters spawnParams;
				AJS_LandTileActor* spawnedTile = GetWorld()->SpawnActor<AJS_LandTileActor>(TileFactory, location, FRotator::ZeroRotator, spawnParams);

				int randomChoice = FMath::RandRange(0, 99);
				
				if (randomChoice < 10) {
					GetWorld()->SpawnActor<AJS_Tree>(TreeFactory, spawnedTile->GetActorLocation(), FRotator::ZeroRotator, spawnParams);
				}
				else if (randomChoice > 10 && randomChoice < 20) {
					GetWorld()->SpawnActor<AJS_Rock>(RockFactory, spawnedTile->GetActorLocation(), FRotator::ZeroRotator, spawnParams);
				}
				else if (randomChoice > 20 && randomChoice < 40) {
					GetWorld()->SpawnActor<AJS_Gress>(GressFactory, spawnedTile->GetActorLocation(), FRotator::ZeroRotator, spawnParams);
				}
			}
		}
	}
}

// Called every frame
void AJS_LandGridActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
