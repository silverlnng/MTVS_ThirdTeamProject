// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_Pumpkin.generated.h"

UCLASS()
class MTVSTHIRDPROJECT_API AJS_Pumpkin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_Pumpkin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USphereComponent* sphereComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* pumpkinMeshComp;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable)
	void Server_OnOverlapBegin(AActor* OtherActor, bool bOverlapBegin);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OnOverlapBegin(AActor* OtherActor, bool bOverlapBegin);
};
