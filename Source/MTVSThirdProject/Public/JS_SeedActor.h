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

	//소환될 녀석을 담는 변수
	UClass* PlantClassToSpawn = nullptr;

	int32 maxHP = 1;
	int32 curHP = maxHP;
	float checkDeltaTime = 0;
	float growTime = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seed")
	bool bSeedActorInteract = true;

	virtual void GetDamage_Implementation(bool damage) override;
	virtual void SetCurHP_Implementation(float amount) override;
	virtual void SpawnNextPlant_Implementation(int32 index) override;
	virtual void Death_Implementation() override;
};
