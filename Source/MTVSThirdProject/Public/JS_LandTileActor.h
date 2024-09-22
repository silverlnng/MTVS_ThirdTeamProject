// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_GridManager.h"
#include "JS_SeedActor.h"
#include "JS_LandTileActor.generated.h"

UCLASS()
class MTVSTHIRDPROJECT_API AJS_LandTileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_LandTileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* landTileComp;
	
	UPROPERTY(EditAnywhere, Category = "SetSeed")
	TSubclassOf<AJS_SeedActor> wheatSeedFactroy;
	UPROPERTY(EditAnywhere, Category = "SetSeed")
	TSubclassOf<AJS_SeedActor> pumpKinSeedFactroy;
	UPROPERTY(EditAnywhere, Category = "SetSeed")
	TSubclassOf<AJS_SeedActor> carrotSeedFactroy;
	UPROPERTY(EditAnywhere, Category = "SetSeed")
	TSubclassOf<AJS_SeedActor> watermelonSeedFactroy;
	UPROPERTY(EditAnywhere, Category = "SetSeed")
	TSubclassOf<AJS_SeedActor> strawberrySeedFactroy;

	//물에 타일이 젖어있는지 확인하는 변수
	bool bIsWet = false;
	//괭이질을 했는지 확인하는 변수
	bool canFraming = false;
	//땅이 건조해지는 걸 체크하는 변수
	float checkDeltaTime = 0;
	// 21600델타타임 == 6분
	int32 dryingTime = 21600;

	AJS_GridManager* GridManager;
	FVector2D GridCoordinates; // 타일 그리드 좌표

	FVector2D GetGridCoordinates() const { return GridCoordinates;} // 타일의 그리드 좌표를 반환하는 함수
	void SetGridCoordinates(FVector2D NewCoordinates) { GridCoordinates = NewCoordinates;} // 타일의 좌표를 설정하는 함수
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void SpawnCrops(int32 id);
};
