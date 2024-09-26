// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_GridManager.h"
#include "JS_LandTileActor.h"
#include "EngineUtils.h"
#include "Components/BoxComponent.h"

// Sets default values
AJS_GridManager::AJS_GridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	boxComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	bReplicates = true;
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

void AJS_GridManager::FindOwner()
{
	if ( HasAuthority() ) {
		AActor* newOwner = nullptr;
		float minDist = searchDistance;

		for ( TActorIterator<AJS_GridManager> it(GetWorld()); it; ++it ) {
			AActor* otherActor = *it;
			float dist = GetDistanceTo(otherActor);

			if ( dist < minDist ) {
				minDist = dist;
				newOwner = otherActor;
			}
		}
		if ( GetOwner() != newOwner ) {
			SetOwner(newOwner);
		}
	}
}

void AJS_GridManager::InitializeGrid(int32 InGridSizeX, int32 InGridSizeY)
{
	gridSizeX = InGridSizeX;
	gridSizeY = InGridSizeY;

	MinGridBounds = FVector2D(0, 0); //최소 좌표는 (0, 0)으로 설정
	MaxGridBounds = FVector2D(gridSizeX - 1, gridSizeY - 1); // 최대 좌표는 그리드 크기 기반으로 설정

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
		//타일이 이미 점유된 경우
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
// 주어진 2D 좌표가 그리드에 있는지 확인하고, 그 좌표가 존재하면 해당 셀이 점유되었는지를 반환한다. 존재하지 않으면 기본적으로 점유되지 않은 것으로 간주하여 false를 반환
bool AJS_GridManager::IsCellOccupied(FVector2D gridCoordinates)
{
	if (Grid.Contains(gridCoordinates)) {
		return Grid[gridCoordinates].bIsOccupied;
	}
	return false;
}
//그리드를 초기화 시키는 함수
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
	//그리드 좌표를 월드 좌표로 변환하는 로직
	float TileSize = 100.0f; // 타일 간 간격 SetGridTiledptj landSpacing 값과 동일해야한다.
	FVector Origin = GetActorLocation(); // 그리드의 시작점 LandGridActor의 위치를 기준으로 할 수 있다.

	//그리드 좌표를 사용해 월드 위치 계산
	float X = Origin.X + GridCoordinates.X * TileSize;
	float Y = Origin.Y + GridCoordinates.Y * TileSize;

	return FVector(X, Y, Origin.Z);
}