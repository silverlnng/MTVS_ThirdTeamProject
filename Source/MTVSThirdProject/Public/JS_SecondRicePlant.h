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
	
	// interact------------------------------------------------
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Sound")
	class USoundBase* getActorSound;
	UPROPERTY(EditAnywhere , BlueprintReadWrite , Category = "Sound")
	class USoundBase* grownPlantSound;
	// interact------------------------------------------------
	
	//소환될 녀석을 담는 변수
	UPROPERTY(Replicated)
	UClass* PlantClassToSpawn = nullptr;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Set")
	int32 maxHP = 1;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Set")
	int32 curHP = maxHP;

	// 플레이어가 상호작용 했을 때 스폰 되게 하는 불값
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bInteractSecondPlant = false;

	// 수확할 때 퀘스트에서 증가해야할 값
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Harvest")
	int32 harvestWheatCount = 0;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Harvest")
	int32 harvestPumpkinCount = 0;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Harvest")
	int32 harvestCarrotCount = 0;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Harvest")
	int32 harvestStrawberryCount = 0;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Harvest")
	int32 harvestWatermelonCount = 0;

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


	//GetDamage RPC
	UFUNCTION(Server, Reliable)
	void Server_GetDamageRPC(bool damage);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_GetDamageRPC(bool damage);

	//SetCurHP RPC
	UFUNCTION(Server, Reliable)
	void Server_SetCurHPRPC(float amount);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetCurHPRPC(float amount);

	//Spawn RPC
	UFUNCTION(Server, Reliable)
	void Server_SpawnNextPlantRPC(int32 index);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpawnNextPlantRPC(int32 index);

	UFUNCTION(Server, Reliable)
	void Server_DeathRPC();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DeathRPC();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
