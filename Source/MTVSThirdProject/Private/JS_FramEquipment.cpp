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
	//���� Ƚ�� üũ
	if(fishingCount <= 3) fishingCount++;
	//15~25�ʻ��̸� �������� ����
	int32 randomTime = FMath::RandRange(15, 25);
	if (currentTime > 25) {
		currentTime = 0;
	}
	//currentTime�� �������ڿ� �������� 3���� �� �ϳ� ȹ��
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
	//������
	if (OtherActor && OtherActor->ActorHasTag(TEXT("LandTile")) && this->ActorHasTag(TEXT("Hoe"))){
		//������ �Ҷ� �Ұ� �޾Ƽ� �� �۹� ���� �� �ְ� ó��
		tileActor->canFraming = true;
	}
	//�絿�� ��ä���
	if (OtherActor && OtherActor->ActorHasTag(TEXT("Well")) && this->ActorHasTag(TEXT("Pail"))) {
		GetHaveWater();
	}
	//����
	if (OtherActor && OtherActor->ActorHasTag(TEXT("Fishing_Spot")) && this->ActorHasTag(TEXT("Fishing_Rod"))) {
		StartFishing();
	}
	// �浹�� ���Ͱ� ����, ��, Ǯ �±׸� ���� ��ֹ��� ���
	if (OtherActor && (OtherActor->ActorHasTag(TEXT("Tree")) || OtherActor->ActorHasTag(TEXT("Rock")) || OtherActor->ActorHasTag(TEXT("Gress")))) {
		AJS_ObstacleActor* obstacleActor = Cast<AJS_ObstacleActor>(OtherActor);
		if (obstacleActor) {
			// ��� Axe, Pick, Sickle �� �ϳ��� �� ó��
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

