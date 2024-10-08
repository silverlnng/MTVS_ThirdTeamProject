// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AJH_FarmTile.generated.h"


UCLASS()
class MTVSTHIRDPROJECT_API AAJH_FarmTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAJH_FarmTile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Replicated, EditDefaultsOnly)
	class UBoxComponent* boxComp;
	UPROPERTY(Replicated, EditDefaultsOnly)
	class UStaticMeshComponent* bodyMesh;

	UFUNCTION(Server, Reliable)
	void ServerChangeCollision();
	UFUNCTION(NetMulticast, Reliable)
	void MultiChangeCollision();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
