﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_FramEquipment.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "JS_ObstacleActor.h"
#include "Net/UnrealNetwork.h"
#include "EngineUtils.h"

// Sets default values
AJS_FramEquipment::AJS_FramEquipment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	framEquipmentComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("framEquipment"));
	framEquipmentComp->SetupAttachment(boxComp);
	framEquipmentComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	bReplicates = true;
	bAlwaysRelevant = true;
}

void AJS_FramEquipment::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//변수에 replicated를 사용한다면 등록해야함.
	DOREPLIFETIME (AJS_FramEquipment, haveWater);

	//낚시 드롭
	/*DOREPLIFETIME (AJS_FramEquipment, currentTime);
	DOREPLIFETIME (AJS_FramEquipment, fishingCount);
	DOREPLIFETIME (AJS_FramEquipment, jewelCount);
	DOREPLIFETIME (AJS_FramEquipment, fishCount);
	DOREPLIFETIME (AJS_FramEquipment, trashCount);*/

}

// Called when the game starts or when spawned
void AJS_FramEquipment::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AJS_FramEquipment::OnOverlapBegin);
	//FindOwner();
}

// Called every frame
void AJS_FramEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//currentTime += DeltaTime;

}

// 농기구를 잡아서 처리할 때 플레이어 쪽에서 하는 게 맞다.
//void AJS_FramEquipment::FindOwner()
//{
//	if ( HasAuthority() ) {
//
//		AActor* newOwner = GetWorld()->GetFirstPlayerController()->GetPawn();
//		if ( GetOwner() != newOwner ) 
//			SetOwner(newOwner);
//		//float minDist = searchDistance;
//
//		//for ( TActorIterator<AJS_FramEquipment> it(GetWorld()); it; ++it ) {
//		//	AActor* otherActor = *it;
//
//		//	// 나 자신은 제외
//		//	if ( this == otherActor )
//		//		continue;
//		//	float dist = GetDistanceTo(otherActor);
//
//		//	if ( dist < minDist ) {
//		//		minDist = dist;
//		//		newOwner = otherActor;
//		//	}
//		//}
//		//if ( GetOwner() != newOwner ) {
//		//	SetOwner(newOwner);
//		//}
//	}
//}

void AJS_FramEquipment::GetHaveWater()
{
	Server_GetHaveWater();
}

void AJS_FramEquipment::Server_GetHaveWater_Implementation()
{
	Multicast_GetHaveWater();
}

void AJS_FramEquipment::Multicast_GetHaveWater_Implementation()
{
	if ( haveWater >= 0 && haveWater < 10 ) {
		haveWater = 10;
		UE_LOG(LogTemp , Warning , TEXT("haveWater = %d") , haveWater);
	}
}

void AJS_FramEquipment::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!OtherActor) return;
	
	
 	APawn* player = GetWorld()->GetFirstPlayerController()->GetPawn();
 
 	if ( player == OtherActor)
 	{
 		SetOwner(player);
 		UE_LOG(LogTemp , Warning , TEXT("playerController : %s") , *player->GetName());
 
 	}

	Server_OverlapBegin(OtherActor);
}
void AJS_FramEquipment::Server_OverlapBegin_Implementation(AActor* OtherActor)
{
	
	if ( !OtherActor )
	{
		UE_LOG(LogTemp , Warning , TEXT("ServerBeginOverlap : null"));
		return;
	}
	//괭이질
	UE_LOG(LogTemp , Warning , TEXT("Server_bIsBeginOverlap In"));
	if ( OtherActor && OtherActor->ActorHasTag(TEXT("LandTile")) && this->ActorHasTag(TEXT("Hoe")) ) {
		//괭이질 할때 불값 받아서 땅 작물 심을 수 있게 처리
		tileActor->canFraming = true;
	}
	//양동이 물채우기
	if ( OtherActor && OtherActor->ActorHasTag(TEXT("Well")) && this->ActorHasTag(TEXT("Pail")) ) {
		GetHaveWater();
	}
	//낚시 드롭
	/*if ( OtherActor && OtherActor->ActorHasTag(TEXT("Fishing_Spot")) && this->ActorHasTag(TEXT("Fishing_Rod")) ) {
		StartFishing();
	}*/

	// 충돌한 액터가 나무, 돌, 풀 태그를 가진 장애물인 경우
	if ( OtherActor && (OtherActor->ActorHasTag(TEXT("Tree")) || OtherActor->ActorHasTag(TEXT("Rock")) || OtherActor->ActorHasTag(TEXT("Gress"))) ) {
		AJS_ObstacleActor* obstacleActor = Cast<AJS_ObstacleActor>(OtherActor);
		if ( obstacleActor ) {
			UE_LOG(LogTemp , Warning , TEXT("Overlap Tree, Rock, Gress"));
			// 장비가 Axe, Pick, Sickle 중 하나일 때 처리
			if ( this->ActorHasTag(TEXT("Axe")) && OtherActor->ActorHasTag(TEXT("Tree")) ) {
				obstacleActor->GetDamage_Implementation(true);
			}
			else if ( this->ActorHasTag(TEXT("Pick")) && OtherActor->ActorHasTag(TEXT("Rock")) ) {
				obstacleActor->GetDamage_Implementation(true);
			}
			else if ( this->ActorHasTag(TEXT("Sickle")) && OtherActor->ActorHasTag(TEXT("Gress")) ) {
				UE_LOG(LogTemp , Warning , TEXT("hit gress"));
				obstacleActor->GetDamage_Implementation(true);
			}
		}
	}
	Multicast_OverlapBegin(OtherActor);
}

void AJS_FramEquipment::Multicast_OverlapBegin_Implementation(AActor* OtherActor){
	if ( !OtherActor )
	{
		UE_LOG(LogTemp , Warning , TEXT("MulticastBeginOverlap: null"));
		return;
	}
}

//void AJS_FramEquipment::StartFishing()
//{
//	Server_StartFishing();
//}
//
//void AJS_FramEquipment::Server_StartFishing_Implementation()
//{
//	Multicast_StartFishing();
//}
//
//void AJS_FramEquipment::Multicast_StartFishing_Implementation()
//{
//	//낚시 횟수 체크
//	if ( fishingCount < 3 ) fishingCount++;
//	//15~25초사이를 랜덤으로 얻어옴
//	int32 randomTime = FMath::RandRange(15 , 25);
//	if ( currentTime > 25 ) {
//		currentTime = 0;
//	}
//	//currentTime이 랜덤숫자와 같아지면 3가지 중 하나 획득
//	if ( randomTime == currentTime ) {
//		int32 randomCount = FMath::RandRange(0 , 99);
//		if ( randomCount < 5 && jewelCount < 1 ) jewelCount++;
//		if ( randomCount >= 5 && randomCount < 35 && fishCount < 3 ) fishCount++;
//		if ( randomCount >= 35 && trashCount < 10 ) trashCount++;
//	}
//}