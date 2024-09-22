// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_LandTileActor.h"
#include "JS_GridManager.h"
#include "Components/BoxComponent.h"

// Sets default values
AJS_LandTileActor::AJS_LandTileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50, 50, 1));
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	boxComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Block);
}

// Called when the game starts or when spawned
void AJS_LandTileActor::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AJS_LandTileActor::OnOverlapBegin);
}

// Called every frame
void AJS_LandTileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//���� ���� ������ ��ŸŸ���� �ʴ���.
	if(bIsWet == false) checkDeltaTime += DeltaTime;
	//�ð��� 6���� ������ ������ �� üũ
	if (checkDeltaTime == dryingTime) {
		checkDeltaTime = 0;
		bIsWet = false;
	}
}

void AJS_LandTileActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GridManager) {
		FVector2D gridCoordinates = GetGridCoordinates();
		GridManager->ClearGridCell(gridCoordinates);
	}
}
void AJS_LandTileActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Overlap Detected in boxComp!"));

	// ����� �޽����� � ���Ͱ� ���ƴ��� ���
	if (OtherActor) {
		FString actorName = OtherActor->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Overlapping with Actor: %s"), *actorName));
	}
}

void AJS_LandTileActor::SpawnCrops(int32 id)
{
	if(!GridManager) return;
	if(canFraming == false || bIsWet == false) return;
	
	//Ÿ�� ���� �Ǿ����� Ȯ��
	FVector2D tileCoords = GetGridCoordinates();
	if (GridManager->IsCellOccupied(tileCoords)) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Tile is already occupied!"));
		return;
	}

	FActorSpawnParameters spawnParmas;
	spawnParmas.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//������ �۹��� ������ ����
	AActor* spawnedCrop = nullptr;  
	switch (id)
	{
		case 11010:
			spawnedCrop = GetWorld()->SpawnActor<AJS_SeedActor>(wheatSeedFactroy, GetActorLocation(), FRotator::ZeroRotator, spawnParmas);
			break;
		case 11011:
			spawnedCrop = GetWorld()->SpawnActor<AJS_SeedActor>(watermelonSeedFactroy, GetActorLocation(), FRotator::ZeroRotator, spawnParmas);
			break;
		case 11012:
			spawnedCrop = GetWorld()->SpawnActor<AJS_SeedActor>(strawberrySeedFactroy, GetActorLocation(), FRotator::ZeroRotator, spawnParmas);
			break;
		case 11013:
			spawnedCrop = GetWorld()->SpawnActor<AJS_SeedActor>(carrotSeedFactroy, GetActorLocation(), FRotator::ZeroRotator, spawnParmas);
			break;
		case 11014:
			spawnedCrop = GetWorld()->SpawnActor<AJS_SeedActor>(pumpKinSeedFactroy, GetActorLocation(), FRotator::ZeroRotator, spawnParmas);
			break;
		default:
			break;
	}
	
	if (spawnedCrop) {
		GridManager->SetObjectAtGridCell(tileCoords, spawnedCrop);
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to spawn crop!"));
	}
}
