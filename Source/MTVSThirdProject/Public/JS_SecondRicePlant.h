// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_DefaultPlant.h"
#include "JS_SecondRicePlant.generated.h"

UCLASS()
class MTVSTHIRDPROJECT_API AJS_SecondRicePlant : public AActor, public IJS_DefaultPlant
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_SecondRicePlant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USphereComponent* sphereComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* sphereMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "fruit")
	TSubclassOf<class AJS_Rice> RiceFactory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "fruit")
	TSubclassOf<class AJS_Pumpkin> PumpkinFactory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "fruit")
	TSubclassOf<class AJS_Carrot> CarrotFactory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "fruit")
	TSubclassOf<class AJS_Strawberry> StrawberryFactory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "fruit")
	TSubclassOf<class AJS_Watermelon> WatermelonFactory;
	
	//소환될 녀석을 담는 변수
	UClass* PlantClassToSpawn = nullptr;

	int32 maxHP = 1;
	int32 curHP = maxHP;
	// 플레이어가 상호작용 했을 때
	bool bInteractSecondPlant = true;

	//각 작물의 ID값
	int32 riceID = 70100;
	int32 watermelonID = 70101;
	int32 strawberryID = 70102;
	int32 carrotID = 70104;
	int32 pumpKinID = 70105;
	

	
	virtual void GetDamage_Implementation(bool damage) override;
	virtual void SetCurHP_Implementation(float amount) override;
	virtual void SpawnNextPlant_Implementation(int32 index) override;
	virtual void Death_Implementation() override;
};
