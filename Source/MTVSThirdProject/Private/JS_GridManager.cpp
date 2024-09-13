// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_GridManager.h"

// Sets default values
AJS_GridManager::AJS_GridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJS_GridManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJS_GridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJS_GridManager::InitializeGrid(int32 InGridSizeX, int32 InGridSizeY)
{
	gridSizeX = InGridSizeX;
	gridSizeY = InGridSizeY;

	MinGridBounds = FVector2D(0, 0); //�ּ� ��ǥ�� (0, 0)���� ����
	MaxGridBounds = FVector2D(gridSizeX - 1, gridSizeY - 1); // �ִ� ��ǥ�� �׸��� ũ�� ������� ����

	for (int32 x = 0; x < gridSizeX; ++x) {
		for (int32 y = 0; y < gridSizeY; ++y) {
			FVector2D gridCoordinates(x, y);
			FGridTile newTile;
			newTile.gridCoordinates = gridCoordinates;
			Grid.Add(gridCoordinates, newTile);
		}
	}
}

void AJS_GridManager::SetObjectAtGridCell(FVector2D gridCoordinates, AActor* Object)
{
	if (Grid.Contains(gridCoordinates)) {
		FGridTile& tile = Grid[gridCoordinates];
		//Ÿ���� �̹� ������ ���
		if (tile.bIsOccupied) {
			UE_LOG(LogTemp, Warning, TEXT("Tile at (%f, %f) is already occupied by %s"),gridCoordinates.X, gridCoordinates.Y,tile.occupyingActor ? *tile.occupyingActor->GetName() : TEXT("Unknown Actor"));
			return;
		}
		tile.occupyingActor = Object;
		tile.bIsOccupied = (Object != nullptr);
	}

}

AActor* AJS_GridManager::GetObjectAtGridCell(FVector2D gridCoordinates)
{
	if (Grid.Contains(gridCoordinates)) {
		return Grid[gridCoordinates].occupyingActor;
	}
	return nullptr;
}
// �־��� 2D ��ǥ�� �׸��忡 �ִ��� Ȯ���ϰ�, �� ��ǥ�� �����ϸ� �ش� ���� �����Ǿ������� ��ȯ�Ѵ�. �������� ������ �⺻������ �������� ���� ������ �����Ͽ� false�� ��ȯ
bool AJS_GridManager::IsCellOccupied(FVector2D gridCoordinates)
{
	if (Grid.Contains(gridCoordinates)) {
		return Grid[gridCoordinates].bIsOccupied;
	}
	return false;
}

void AJS_GridManager::ClearGridCell(FVector2D gridCoordinates)
{
	if (Grid.Contains(gridCoordinates)) {
		FGridTile& Tile = Grid[gridCoordinates];
		Tile.occupyingActor = nullptr;
		Tile.bIsOccupied = false;
	}
}

FVector AJS_GridManager::GetTileLocation(FVector2D GridCoordinates)
{
	//�׸��� ��ǥ�� ���� ��ǥ�� ��ȯ�ϴ� ����
	float TileSize = 100.0f; // Ÿ�� �� ���� SetGridTiledptj landSpacing ���� �����ؾ��Ѵ�.
	FVector Origin = GetActorLocation(); // �׸����� ������ LandGridActor�� ��ġ�� �������� �� �� �ִ�.

	//�׸��� ��ǥ�� ����� ���� ��ġ ���
	float X = Origin.X + GridCoordinates.X * TileSize;
	float Y = Origin.Y + GridCoordinates.Y * TileSize;

	return FVector(X, Y, Origin.Z);
}

