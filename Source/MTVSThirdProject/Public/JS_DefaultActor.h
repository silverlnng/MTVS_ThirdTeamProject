// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_DefaultActor.generated.h"

UCLASS()
class MTVSTHIRDPROJECT_API AJS_DefaultActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_DefaultActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float getGrowTime = 0;

	UFUNCTION(BlueprintCallable)
	void SpawnPlantActor(float growTime, bool bGetWater, int32 index);

	UFUNCTION(BlueprintCallable)
	void SetCurHP(float damage);

	UFUNCTION(BlueprintCallable)
	void SpawnTreeActor();

	UFUNCTION(BlueprintCallable)
	void SpawnGrassActor();
};
