// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_GridManager.generated.h"

USTRUCT(BlueprintType)
struct FGridTile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D gridCoordinates; // Ÿ���� ��ǥ�� ��Ÿ����.
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* occupyingActor; // Ÿ�Ͽ� ��ġ�� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsOccupied; // Ÿ���� ���� �����Ǿ����� ���θ� ��Ÿ����.

	FGridTile() : gridCoordinates(FVector2D::ZeroVector), occupyingActor(nullptr), bIsOccupied(false) {}
};

UCLASS()
class MTVSTHIRDPROJECT_API AJS_GridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_GridManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FVector2D, FGridTile> Grid;

	int32 gridSizeX;
	int32 gridSizeY;
	FVector2D MinGridBounds;
	FVector2D MaxGridBounds;

	UFUNCTION(BlueprintCallable)
	void InitializeGrid(int32 InGridSizeX, int32 InGridSizeY);
	UFUNCTION(BlueprintCallable)
	void SetObjectAtGridCell(FVector2D gridCoordinates, AActor* Object);
	UFUNCTION(BlueprintCallable)
	AActor* GetObjectAtGridCell(FVector2D gridCoordinates);
	UFUNCTION(BlueprintCallable)
	bool IsCellOccupied(FVector2D gridCoordinates);
	void ClearGridCell(FVector2D gridCoordinates);
	FVector GetTileLocation(FVector2D GridCoordinates);

};
