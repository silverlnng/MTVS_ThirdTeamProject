// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_Watermelon.generated.h"

UCLASS()
class MTVSTHIRDPROJECT_API AJS_Watermelon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_Watermelon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class USphereComponent* sphereComp;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* carrotMeshComp;
};
