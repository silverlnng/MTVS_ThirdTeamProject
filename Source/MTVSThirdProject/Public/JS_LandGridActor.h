// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_GridManager.h"
#include "JS_LandGridActor.generated.h"

UCLASS()
class MTVSTHIRDPROJECT_API AJS_LandGridActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_LandGridActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// 몇 곱하기 몇으로 할지 gridSize 설정.
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Grid")
	int32 gridSize = 20;
	// 각 액터마다 얼마나 떨어져 있을 건지 설정.
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Grid")
	float landSpacing = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AJS_LandTileActor> TileFactory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AJS_ObstacleActor> TreeFactory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AJS_ObstacleActor> RockFactory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AJS_ObstacleActor> GressFactory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AJS_GridManager* GridManager;
	AJS_LandTileActor* TileActor;

	void SetGridTile();
	//bool IsActorOutOfBounds(AActor* Actor);
	//void PlaceActorOnTile(FVector2D GridCoordinates, AActor* ActorToPlace);
	void FindActorsOnTile(FVector2D GridCoordinates, TArray<AActor*>& OutOverlappingActors);

	UFUNCTION(Server, Reliable)
	void Server_SetGridTile();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetGridTile();
};
