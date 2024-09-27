// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_LandGridActor.h"
#include "JS_LandTileActor.h"
#include "Components/BoxComponent.h"
#include "JS_ObstacleActor.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AJS_LandGridActor::AJS_LandGridActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
}

void AJS_LandGridActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//변수에 replicated를 사용한다면 등록해야함.
	DOREPLIFETIME (AJS_LandGridActor, gridSize);
	DOREPLIFETIME (AJS_LandGridActor, landSpacing);
}

// Called when the game starts or when spawned
void AJS_LandGridActor::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if ( playerController ) {
		SetOwner(playerController);
	}

	if (!GridManager) {
		GridManager = GetWorld()->SpawnActor<AJS_GridManager>(AJS_GridManager::StaticClass());
	}
	if (GridManager) {
		//그리드 관리 시스템 초기화
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

	/*FVector2D queryCoordinates(0, 0);
	AActor* objectAtTile = GridManager->GetObjectAtGridCell(queryCoordinates);
	bool checkTile = GridManager->IsCellOccupied(queryCoordinates);*/
	
	//그리드 배열 순회
	for (auto& Elem : GridManager->Grid) { //Elem 각 그리드 요소 Elem.Value는 타일 정보를 가져온다(FGridTile)
		FGridTile& Tile = Elem.Value;// Elem.Value를 참조해서 현재 타일(FGridTile)을 가져옴. Tile변수는 현재 처리 중인 타일에 대한 참조
		
		TArray<AActor*> OverlappingActors; // 현재 타일과 겹치는 액터들을 저장하기 위한 배열
		FindActorsOnTile(Tile.gridCoordinates, OverlappingActors); //타일 좌표에 해당하는 타일에 겹치는 액터들을 찾고 그 액터들을 OverlappingActors 배열에 채워 넣는다.
		//Tile.girdCoordinates를 받아서 그 타일 위에 있는 액터들을 찾는 역할을 한다.
		
		for (AActor* Actor : OverlappingActors) { // OverlappingActors배열을 순회하면서 해당 타일에 겹쳐 있는 모든 액터들을 하나씩 처리합니다.
			if (!Tile.occupyingActor) // 현재 타일에 오브젝트가 할당되지 않은 경우(occupyingActor가 nullptr인 경우)만 오브젝트를 할당
			{
				Tile.occupyingActor = Actor; // 현재 타일에 겹쳐 있는 첫 번째 액터를 occupyingActor로 설정해서 그 타일 위를 점령한다.
				Tile.bIsOccupied = true; // true로 만들어서 타일이 점령 되어있다는 것을 알림.
				GridManager->SetObjectAtGridCell(Tile.gridCoordinates, Actor); // 그리드 매니저에서 현재 타일 좌표에 해당하는 그리드 셀에 액터를 할당한다. 그리드 시스템 내에서 해당 셀에 어떤 오브젝트가 배치되었는지 기록하는 역할을 한다.
			}
		}
		
	}
}

//bool AJS_LandGridActor::IsActorOutOfBounds(AActor* Actor)
//{
//	if (GridManager) {
//		FVector ActorLocation = Actor->GetActorLocation();
//
//		float MinX = GridManager->MinGridBounds.X;
//		float MinY = GridManager->MinGridBounds.Y;
//		float MaxX = GridManager->MaxGridBounds.X;
//		float MaxY = GridManager->MaxGridBounds.Y;
//
//		//타일 범위 조건을 설정하여 액터가 범위를 벗어났는지 확인
//		return (ActorLocation.X < MinX || ActorLocation.X > MaxX || ActorLocation.Y  < MinY || ActorLocation.Y > MaxY);
//	}
//	return true; // GridManager가 없으면 기본적으로 범위를 벗어난 것으로 간주
//	
//}

//void AJS_LandGridActor::PlaceActorOnTile(FVector2D GridCoordinates, AActor* ActorToPlace)
//{
//	if (GridManager && ActorToPlace) {
//		FVector TileLocation = GridManager->GetTileLocation(GridCoordinates); //타일 위치 얻기
//		ActorToPlace->SetActorLocation(TileLocation);
//		GridManager->SetObjectAtGridCell(GridCoordinates, ActorToPlace); // 그리드에 등록
//	}
//}

void AJS_LandGridActor::SetGridTile()
{
	Server_SetGridTile();
}

void AJS_LandGridActor::Server_SetGridTile_Implementation()
{
	Multicast_SetGridTile();
}

void AJS_LandGridActor::Multicast_SetGridTile_Implementation()
{
	//GridManager를 가져와서 그리드 크기 초기화
		//스폰된 오브젝트를 저장한 변수 선언.
	for ( int32 x = 0; x < gridSize; ++x ) {
		for ( int32 y = 0; y < gridSize; ++y ) {
			FVector location = GetActorLocation() + FVector(x * landSpacing , y * landSpacing , 0.0f);

			if ( TileFactory ) {
				FActorSpawnParameters spawnParams;
				spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding; // 액터가 있으면 스폰 안시키는 파라미터
				AJS_LandTileActor* spawnedTile = GetWorld()->SpawnActor<AJS_LandTileActor>(TileFactory , location , FRotator::ZeroRotator , spawnParams);

				if ( spawnedTile ) {
					spawnedTile->SetGridCoordinates(FVector2D(x , y));

					int randomChoice = FMath::RandRange(0 , 399);

					AActor* spawnedObject = nullptr; //스폰된 오브젝트를 저장할 변수

					if ( randomChoice < 5 && randomChoice > 0 ) {
						spawnedObject = GetWorld()->SpawnActor<AJS_ObstacleActor>(TreeFactory , spawnedTile->GetActorLocation() + (GetActorUpVector() * 50) , FRotator::ZeroRotator , spawnParams);
					}
					else if ( randomChoice < 10 && randomChoice >= 5 ) {
						spawnedObject = GetWorld()->SpawnActor<AJS_ObstacleActor>(RockFactory , spawnedTile->GetActorLocation() + (GetActorUpVector() * 50) , FRotator::ZeroRotator , spawnParams);
						if ( spawnedObject )
						{
							UE_LOG(LogTemp , Warning , TEXT("Not null to spawnRock"));
							spawnedObject;
						}
						else {
							UE_LOG(LogTemp , Warning , TEXT("Failed to spawn Rock"));
						}
					}
					else if ( randomChoice < 30 && randomChoice > 10 ) {
						spawnedObject = GetWorld()->SpawnActor<AJS_ObstacleActor>(GressFactory , spawnedTile->GetActorLocation() + (GetActorUpVector() * 50) , FRotator::ZeroRotator , spawnParams);
					}

					//오브젝트가 스폰된 경우 그리드에 기록
					if ( spawnedObject && GridManager ) {
						FVector2D gridCoordinates(x , y);
						GridManager->SetObjectAtGridCell(gridCoordinates , spawnedObject);
					}
				}
				else {
					UE_LOG(LogTemp , Warning , TEXT("Failed to spawn tile at coordinates: (%d, %d)") , x , y);
				}
			}
		}
	}
}

void AJS_LandGridActor::FindActorsOnTile(FVector2D GridCoordinates , TArray<AActor*>& OutOverlappingActors)
{
	if ( GridManager ) {
		FVector TileLocation = GridManager->GetTileLocation(GridCoordinates);

		// 타일의 위치에서 해당 타일 액터를 찾습니다.
		TileActor = Cast<AJS_LandTileActor>(GridManager->GetObjectAtGridCell(GridCoordinates));
		if ( TileActor && TileActor->boxComp ) {
			//타일 컴포넌트에서 겹치는 액터를 검색
			GEngine->AddOnScreenDebugMessage(-1 , 3.f , FColor::Blue , TEXT("Checking for overlapping actors..."));
			TileActor->boxComp->GetOverlappingActors(OutOverlappingActors);

			UE_LOG(LogTemp , Warning , TEXT("Found %d overlapping actors on tile at (%f, %f)") , OutOverlappingActors.Num() , GridCoordinates.X , GridCoordinates.Y);
		}
	}
}