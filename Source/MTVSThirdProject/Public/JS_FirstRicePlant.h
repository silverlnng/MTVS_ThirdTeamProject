// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_DefaultPlant.h"
#include "JS_FirstRicePlant.generated.h"

UCLASS()
class MTVSTHIRDPROJECT_API AJS_FirstRicePlant : public AActor, public IJS_DefaultPlant
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_FirstRicePlant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	int32 maxHP = 1;
	int32 curHP = maxHP;
	float checkDeltaTime = 0;
	float growTime = 5;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* boxMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SecondPlant")
	TSubclassOf<class AJS_SecondRicePlant> SpawnSecondRicePlant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SecondPlant")
	TSubclassOf<class AJS_SecondRicePlant> SpawnSecondPumpkinPlant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SecondPlant")
	TSubclassOf<class AJS_SecondRicePlant> SpawnSecondCarrotPlant;
	//소환될 녀석을 담는 변수
	UClass* PlantClassToSpawn = nullptr;

	virtual void GetDamage_Implementation(bool damage) override;
	virtual void SetCurHP_Implementation(float amount) override;
	virtual void SpawnNextPlant_Implementation(int32 index) override;
	virtual void Death_Implementation() override;
	
};
