// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_FramEquipment.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "JS_ObstacleActor.h"

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

}

// Called when the game starts or when spawned
void AJS_FramEquipment::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AJS_FramEquipment::OnOverlapBegin);
}

// Called every frame
void AJS_FramEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentTime += DeltaTime;
}

void AJS_FramEquipment::StartFishing()
{
	//낚시 횟수 체크
	if(fishingCount <= 3) fishingCount++;
	//15~25초사이를 랜덤으로 얻어옴
	int32 randomTime = FMath::RandRange(15, 25);
	if (currentTime > 25) {
		currentTime = 0;
	}
	//currentTime이 랜덤숫자와 같아지면 3가지 중 하나 획득
	if (randomTime == currentTime) {
		int32 randomCount = FMath::RandRange(0, 99);
		if(randomCount < 5 && jewelCount < 1) jewelCount++;
		if(randomCount >= 5 && randomCount < 35 && fishCount < 3) fishCount++;
		if(randomCount >= 35 && trashCount < 10) trashCount++;
	}
}

void AJS_FramEquipment::GetHaveWater()
{
	if (haveWater >= 0 && haveWater < 10) haveWater = 10;
}

void AJS_FramEquipment::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//괭이질
	if (OtherActor && OtherActor->ActorHasTag(TEXT("LandTile")) && this->ActorHasTag(TEXT("Hoe"))){
		//괭이질 할때 불값 받아서 땅 작물 심을 수 있게 처리
		tileActor->canFraming = true;
	}
	//양동이 물채우기
	if (OtherActor && OtherActor->ActorHasTag(TEXT("Well")) && this->ActorHasTag(TEXT("Pail"))) {
		GetHaveWater();
	}
	//낚시
	if (OtherActor && OtherActor->ActorHasTag(TEXT("Fishing_Spot")) && this->ActorHasTag(TEXT("Fishing_Rod"))) {
		StartFishing();
	}
	// 충돌한 액터가 나무, 돌, 풀 태그를 가진 장애물인 경우
	if (OtherActor && (OtherActor->ActorHasTag(TEXT("Tree")) || OtherActor->ActorHasTag(TEXT("Rock")) || OtherActor->ActorHasTag(TEXT("Gress")))) {
		AJS_ObstacleActor* obstacleActor = Cast<AJS_ObstacleActor>(OtherActor);
		if (obstacleActor) {
			// 장비가 Axe, Pick, Sickle 중 하나일 때 처리
			if (this->ActorHasTag(TEXT("Axe")) && OtherActor->ActorHasTag(TEXT("Tree"))) {
				obstacleActor->GetDamage_Implementation(true);
			}
			else if (this->ActorHasTag(TEXT("Pick")) && OtherActor->ActorHasTag(TEXT("Rock"))) {
				obstacleActor->GetDamage_Implementation(true);
			}
			else if (this->ActorHasTag(TEXT("Sickle")) && OtherActor->ActorHasTag(TEXT("Gress"))) {
				UE_LOG(LogTemp, Warning, TEXT("hit gress"));
				obstacleActor->GetDamage_Implementation(true);
			}
		}
	}
}

