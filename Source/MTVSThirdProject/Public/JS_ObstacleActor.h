// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_DefaultPlant.h"
#include "JS_ObstacleActor.generated.h"

UCLASS()
class MTVSTHIRDPROJECT_API AJS_ObstacleActor : public AActor, public IJS_DefaultPlant
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_ObstacleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* staticMeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actors")
	TSubclassOf<class AJS_ObstacleActor> TreeFactory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actors")
	TSubclassOf<class AJS_ObstacleActor> GressFactory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actors")
	TSubclassOf<class AJS_ObstacleActor> RockFactory;

	int32 maxHP = 1;
	int32 curHP = maxHP;

	virtual void GetDamage_Implementation(bool damage) override;
	virtual void SetCurHP_Implementation(float amount) override;
	virtual void Death_Implementation() override;
	virtual void SpawnNextPlant_Implementation(int32 index) override;
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
