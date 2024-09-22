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

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "SetPlant")
	int32 maxHP = 1;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "SetPlant")
	int32 curHP = maxHP;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "SetPlant")
	float checkDeltaTime = 0;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "SetPlant")
	float growTime = 5;

	//줄기의 ID값
	int32 ricePlantID = 11210;
	int32 watermelonPlantID = 11211;
	int32 strawberryPlantID = 11212;
	int32 carrotPlantID = 11213;
	int32 pumpKinPlantID = 11214;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SecondPlant")
	TSubclassOf<class AJS_SecondRicePlant> SpawnSecondStrawberryPlant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SecondPlant")
	TSubclassOf<class AJS_SecondRicePlant> SpawnSecondWatermelonPlant;
	//소환될 녀석을 담는 변수
	UPROPERTY(Replicated)
	UClass* PlantClassToSpawn = nullptr;

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
