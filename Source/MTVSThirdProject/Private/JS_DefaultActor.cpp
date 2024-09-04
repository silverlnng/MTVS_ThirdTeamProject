// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_DefaultActor.h"

// Sets default values
AJS_DefaultActor::AJS_DefaultActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJS_DefaultActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJS_DefaultActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	getGrowTime += DeltaTime;
}

void AJS_DefaultActor::SpawnPlantActor(float growTime, bool bGetWater, int32 index)
{
	// ������ 30��, Ǯ�� 10��
	// ���� �� ���¶�� �ڶ󳭴�.
	if (bGetWater) {
		switch (index) {
			case 0:
				if(growTime == 1800) SpawnTreeActor(); // 30�ʰ� ������ ���� ����
				break;
			case 1:	
				if(growTime == 600) SpawnGrassActor(); // 10�ʰ� ������ Ǯ ����
				break;
			default:
				break;
		}
	}
}

void AJS_DefaultActor::SetCurHP(float damage)
{
	
}

void AJS_DefaultActor::SpawnTreeActor()
{
	
}

void AJS_DefaultActor::SpawnGrassActor()
{
	
}

