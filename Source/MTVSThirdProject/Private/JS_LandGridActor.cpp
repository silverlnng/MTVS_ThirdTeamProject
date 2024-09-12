// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_LandGridActor.h"
#include "JS_LandTileActor.h"
#include "Components/BoxComponent.h"
#include "JS_ObstacleActor.h"

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
	
	if (!GridManager) {
		GridManager = GetWorld()->SpawnActor<AJS_GridManager>(AJS_GridManager::StaticClass());
	}
	if (GridManager) {
		//�׸��� ���� �ý��� �ʱ�ȭ
		GridManager->InitializeGrid(gridSize, gridSize);
		SetGridTile();
	}

}

// Called every frame
void AJS_LandGridActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!GridManager) return;
	if (GridManager->Grid.Num() == 0) return;

	FVector2D queryCoordinates(0, 0);
	AActor* objectAtTile = GridManager->GetObjectAtGridCell(queryCoordinates);
	bool checkTile = GridManager->IsCellOccupied(queryCoordinates);
	if (checkTile) {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("This Tile Something IN"));
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Tile is empty"));
	}
	if (objectAtTile) {
		//UE_LOG(LogTemp, Warning, TEXT("Object found at tile (3, 5): %s"), *objectAtTile->GetName());
	}
	else {
		//UE_LOG(LogTemp, Warning, TEXT("Tile (3, 5) is empty."));
	}
	for (auto& Elem : GridManager->Grid) {
		FGridTile& Tile = Elem.Value;
		
		TArray<AActor*> OverlappingActors;
		FindActorsOnTile(Tile.gridCoordinates, OverlappingActors);
		
		for (AActor* Actor : OverlappingActors) {
			if (!Tile.occupyingActor)
			{
				Tile.occupyingActor = Actor;
				Tile.bIsOccupied = true;
				GridManager->SetObjectAtGridCell(Tile.gridCoordinates, Actor);
			}
		}
		
	}
}

void AJS_LandGridActor::SetGridTile()
{
	//GridManager�� �����ͼ� �׸��� ũ�� �ʱ�ȭ
		//������ ������Ʈ�� ������ ���� ����.
	for (int32 x = 0; x < gridSize; ++x) {
		for (int32 y = 0; y < gridSize; ++y) {
			FVector location = GetActorLocation() + FVector(x * landSpacing, y * landSpacing, 0.0f);

			if (TileFactory) {
				FActorSpawnParameters spawnParams;
				spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding; // ���Ͱ� ������ ���� �Ƚ�Ű�� �Ķ����
				AJS_LandTileActor* spawnedTile = GetWorld()->SpawnActor<AJS_LandTileActor>(TileFactory, location, FRotator::ZeroRotator, spawnParams);

				if (spawnedTile) {
					spawnedTile->SetGridCoordinates(FVector2D(x, y));

					int randomChoice = FMath::RandRange(0, 399);

					AActor* spawnedObject = nullptr; //������ ������Ʈ�� ������ ����

					if (randomChoice < 10 && randomChoice > 0) {
						spawnedObject = GetWorld()->SpawnActor<AJS_ObstacleActor>(TreeFactory, spawnedTile->GetActorLocation() + (GetActorUpVector() * 50), FRotator::ZeroRotator, spawnParams);
					}
					else if (randomChoice < 20 && randomChoice >= 10) {
						spawnedObject = GetWorld()->SpawnActor<AJS_ObstacleActor>(RockFactory, spawnedTile->GetActorLocation() + (GetActorUpVector() * 50), FRotator::ZeroRotator, spawnParams);
						if (spawnedObject)
						{
							UE_LOG(LogTemp, Warning, TEXT("Not null to spawnRock"));
							spawnedObject;
						}
						else {
							UE_LOG(LogTemp, Warning, TEXT("Failed to spawn Rock"));
						}
					}
					else if (randomChoice < 40 && randomChoice > 20) {
						 spawnedObject = GetWorld()->SpawnActor<AJS_ObstacleActor>(GressFactory, spawnedTile->GetActorLocation() + (GetActorUpVector() * 50), FRotator::ZeroRotator, spawnParams);
					}

					//������Ʈ�� ������ ��� �׸��忡 ���
					if (spawnedObject && GridManager) {
						FVector2D gridCoordinates(x, y);
						GridManager->SetObjectAtGridCell(gridCoordinates, spawnedObject);
					}
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("Failed to spawn tile at coordinates: (%d, %d)"), x, y);
				}
			}
		}
	}
}

bool AJS_LandGridActor::IsActorOutOfBounds(AActor* Actor)
{
	if (GridManager) {
		FVector ActorLocation = Actor->GetActorLocation();

		float MinX = GridManager->MinGridBounds.X;
		float MinY = GridManager->MinGridBounds.Y;
		float MaxX = GridManager->MaxGridBounds.X;
		float MaxY = GridManager->MaxGridBounds.Y;

		//Ÿ�� ���� ������ �����Ͽ� ���Ͱ� ������ ������� Ȯ��
		return (ActorLocation.X < MinX || ActorLocation.X > MaxX || ActorLocation.Y  < MinY || ActorLocation.Y > MaxY);
	}
	return true; // GridManager�� ������ �⺻������ ������ ��� ������ ����
	
}

void AJS_LandGridActor::PlaceActorOnTile(FVector2D GridCoordinates, AActor* ActorToPlace)
{
	if (GridManager && ActorToPlace) {
		FVector TileLocation = GridManager->GetTileLocation(GridCoordinates); //Ÿ�� ��ġ ���
		ActorToPlace->SetActorLocation(TileLocation);
		GridManager->SetObjectAtGridCell(GridCoordinates, ActorToPlace); // �׸��忡 ���
	}
}

void AJS_LandGridActor::FindActorsOnTile(FVector2D GridCoordinates, TArray<AActor*>& OutOverlappingActors)
{
	if (GridManager) {
		FVector TileLocation = GridManager->GetTileLocation(GridCoordinates);

		// Ÿ���� ��ġ���� �ش� Ÿ�� ���͸� ã���ϴ�.
		TileActor = Cast<AJS_LandTileActor>(GridManager->GetObjectAtGridCell(GridCoordinates));
		if (TileActor && TileActor->boxComp) {
			//Ÿ�� ������Ʈ���� ��ġ�� ���͸� �˻�
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("Checking for overlapping actors..."));
			TileActor->boxComp->GetOverlappingActors(OutOverlappingActors);

			UE_LOG(LogTemp, Warning, TEXT("Found %d overlapping actors on tile at (%f, %f)"), OutOverlappingActors.Num(), GridCoordinates.X, GridCoordinates.Y);
		}
		/*else {
			UE_LOG(LogTemp, Warning, TEXT("TileActor or landTileComp is null at (%f, %f)"), GridCoordinates.X, GridCoordinates.Y);
		}*/
	}
}
