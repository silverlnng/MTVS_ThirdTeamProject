// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_DefaultPlant.h"
#include "JS_SeedActor.generated.h"

UCLASS()
class MTVSTHIRDPROJECT_API AJS_SeedActor : public AActor, public IJS_DefaultPlant
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_SeedActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* seedMeshComp;

	//한 클래스에서 여러 BP액터를 스폰할 수 있게 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FirstPlant")
	TSubclassOf<class AJS_FirstRicePlant> SpawnFirstRicePlant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FirstPlant")
	TSubclassOf<class AJS_FirstRicePlant> SpawnFirstPumpkinPlant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FirstPlant")
	TSubclassOf<class AJS_FirstRicePlant> SpawnFirstCarrotPlant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FirstPlant")
	TSubclassOf<class AJS_FirstRicePlant> SpawnFirstStrawberryPlant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FirstPlant")
	TSubclassOf<class AJS_FirstRicePlant> SpawnFirstWatermelonPlant;

	//소환될 녀석을 담는 변수
	UClass* PlantClassToSpawn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seed")
	int32 maxHP = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seed")
	int32 curHP = maxHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seed")
	float checkDeltaTime = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seed")
	float growTime = 5;
	
	//씨앗의 ID값
	int32 riceID = 11010;
	int32 watermelonID = 11011;
	int32 strawberryID = 11012;
	int32 carrotID = 11013;
	int32 pumpKinID = 11014;
	


	virtual void GetDamage_Implementation(bool damage) override;
	virtual void SetCurHP_Implementation(float amount) override;
	virtual void SpawnNextPlant_Implementation(int32 index) override;
	virtual void Death_Implementation() override;
};
