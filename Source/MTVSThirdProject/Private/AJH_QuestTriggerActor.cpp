// Fill out your copyright notice in the Description page of Project Settings.


#include "AJH_QuestTriggerActor.h"

#include "AJH_Player.h"
#include "Components/BoxComponent.h"
#include "../YJ/NPCWidget.h"
#include "../YJ/YJHUD.h"
#include "../YJ/YJPlayerController.h"

// Sets default values
AAJH_QuestTriggerActor::AAJH_QuestTriggerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(boxComp);
	boxComp->SetCollisionProfileName(TEXT("Pawn"));

}

// Called when the game starts or when spawned
void AAJH_QuestTriggerActor::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AAJH_QuestTriggerActor::OnBeginOverlap);

}

// Called every frame
void AAJH_QuestTriggerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAJH_QuestTriggerActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{
	player = Cast<AAJH_Player>(OtherActor);
	if ( player && player->IsLocallyControlled())
	{
		player->SuccessQuest();
		this->Destroy();
	}

	// 로컬플레이어 의 함수 실행시키기 
}


