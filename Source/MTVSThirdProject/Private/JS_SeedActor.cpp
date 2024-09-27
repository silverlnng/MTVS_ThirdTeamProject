// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_SeedActor.h"
#include "JS_FirstRicePlant.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AJS_SeedActor::AJS_SeedActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50));
	boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	seedMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("seedMeshComp"));
	seedMeshComp->SetupAttachment(boxComp);
	seedMeshComp->SetRelativeScale3D(FVector(1, 1, 0.1f));
	seedMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	ConstructorHelpers::FObjectFinder<USoundBase> plantASeedSoundTemp(TEXT("/Script/Engine.SoundWave'/Game/JS/Sound/InteractSound/PlantASeed.PlantASeed'"));
	if ( plantASeedSoundTemp.Succeeded() ) plantASeedSound = plantASeedSoundTemp.Object;

	bReplicates = true;
}

void AJS_SeedActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME (AJS_SeedActor , PlantClassToSpawn);
	DOREPLIFETIME (AJS_SeedActor , maxHP);
	DOREPLIFETIME (AJS_SeedActor , curHP);
	DOREPLIFETIME (AJS_SeedActor , checkDeltaTime);
	DOREPLIFETIME (AJS_SeedActor , growTime);
	DOREPLIFETIME (AJS_SeedActor , planting_Wheat_SeedCount);
	DOREPLIFETIME (AJS_SeedActor , planting_Pumpkin_SeedCount);
	DOREPLIFETIME (AJS_SeedActor , planting_Carrot_SeedCount);
	DOREPLIFETIME (AJS_SeedActor , planting_Strawberry_SeedCount);
	DOREPLIFETIME (AJS_SeedActor , planting_Watermelon_SeedCount);
}

// Called when the game starts or when spawned
void AJS_SeedActor::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if ( playerController ) {
		SetOwner(playerController);
	}
	UGameplayStatics::PlaySoundAtLocation(this , plantASeedSound , GetActorLocation());
}

// Called every frame
void AJS_SeedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	checkDeltaTime += DeltaTime;

	if (HasAuthority() && checkDeltaTime >= growTime) {
		//상호작용은 한 번만
		if (this->ActorHasTag(TEXT("Wheat"))) {
			//씨를 파괴하고 벼 작물로 스폰
			if( planting_Wheat_SeedCount < 5) planting_Wheat_SeedCount++;
			UE_LOG(LogTemp, Warning, TEXT("This actor has the Wheat tag."));
			SpawnNextPlant_Implementation(riceID);
			GetDamage_Implementation(true);
		}
		if(this->ActorHasTag(TEXT("PumpKin"))) {
			if ( planting_Pumpkin_SeedCount < 5 ) planting_Pumpkin_SeedCount++;
			UE_LOG(LogTemp, Warning, TEXT("This actor has the PumpKin tag."));
			SpawnNextPlant_Implementation(pumpKinID);
			GetDamage_Implementation(true);
		}
		if (this->ActorHasTag(TEXT("Carrot"))) {
			if ( planting_Carrot_SeedCount < 5 ) planting_Carrot_SeedCount++;
			UE_LOG(LogTemp, Warning, TEXT("This actor has the Carrot tag."));
			SpawnNextPlant_Implementation(carrotID);
			GetDamage_Implementation(true);
		}
		if (this->ActorHasTag(TEXT("Strawberry"))) {
			if ( planting_Strawberry_SeedCount < 5 ) planting_Strawberry_SeedCount++;
			UE_LOG(LogTemp, Warning, TEXT("This actor has the Strawberry tag."));
			SpawnNextPlant_Implementation(strawberryID);
			GetDamage_Implementation(true);
		}
		if (this->ActorHasTag(TEXT("Watermelon"))) {
			if ( planting_Watermelon_SeedCount < 5 ) planting_Watermelon_SeedCount++;
			UE_LOG(LogTemp, Warning, TEXT("This actor has the Watermelon tag."));
			SpawnNextPlant_Implementation(watermelonID);
			GetDamage_Implementation(true);
		}
	}
}

void AJS_SeedActor::GetDamage_Implementation(bool damage)
{
	Server_GetDamageRPC(damage);
}

void AJS_SeedActor::SetCurHP_Implementation(float amount)
{
	Server_SetCurHPRPC(amount);
}

void AJS_SeedActor::SpawnNextPlant_Implementation(int32 index)
{
	Server_SpawnNextPlantRPC(index);
}

void AJS_SeedActor::Death_Implementation()
{
	Server_DeathRPC();
}

// GetDamageRPC ------------------------------------------------------------------
void AJS_SeedActor::Server_GetDamageRPC_Implementation(bool damage)
{
	Multicast_GetDamageRPC(damage);
}
void AJS_SeedActor::Multicast_GetDamageRPC_Implementation(bool damage)
{
	if ( damage ) SetCurHP_Implementation(curHP - 1);
}
// GetDamageRPC ------------------------------------------------------------------

// SetCurHPRPC ------------------------------------------------------------------
void AJS_SeedActor::Server_SetCurHPRPC_Implementation(float amount)
{
	Multicast_SetCurHPRPC(amount);
}
void AJS_SeedActor::Multicast_SetCurHPRPC_Implementation(float amount)
{
	if ( amount <= 0 ) Death_Implementation();
	else curHP = amount;
}
// SetCurHPRPC ------------------------------------------------------------------

// SpawnNextPlantRPC ------------------------------------------------------------------
void AJS_SeedActor::Server_SpawnNextPlantRPC_Implementation(int32 index)
{
	Multicast_SpawnNextPlantRPC(index);
}
void AJS_SeedActor::Multicast_SpawnNextPlantRPC_Implementation(int32 index)
{
	if ( HasAuthority() ) {
		switch ( index )
		{
		case 11010:
			PlantClassToSpawn = SpawnFirstRicePlant;
			UE_LOG(LogTemp , Warning , TEXT("id : 11110 firstRicePlant spawn"));
			break;
		case 11011:
			PlantClassToSpawn = SpawnFirstWatermelonPlant;
			UE_LOG(LogTemp , Warning , TEXT("id : 11011 firstRicePlant spawn"));
			break;
		case 11012:
			PlantClassToSpawn = SpawnFirstStrawberryPlant;
			UE_LOG(LogTemp , Warning , TEXT("id : 11012 firstRicePlant spawn"));
			break;
		case 11013:
			PlantClassToSpawn = SpawnFirstCarrotPlant;
			UE_LOG(LogTemp , Warning , TEXT("id : 11013 firstRicePlant spawn"));
			break;
		case 11014:
			PlantClassToSpawn = SpawnFirstPumpkinPlant;
			UE_LOG(LogTemp , Warning , TEXT("id : 11014 firstRicePlant spawn"));
			break;
		default:
			break;
		}

		if ( PlantClassToSpawn ) {
			//선택된 블루프린트 클래스로 액터 스폰
			AJS_FirstRicePlant* SpawnPlant = GetWorld()->SpawnActor<AJS_FirstRicePlant>(PlantClassToSpawn , GetActorLocation() , FRotator::ZeroRotator);
			if ( SpawnPlant ) {
				UGameplayStatics::PlaySoundAtLocation(this, plantASeedSound, GetActorLocation());
				SpawnPlant;
			}
			else UE_LOG(LogTemp , Warning , TEXT("Failed to spawn AJS_FirstRicePlant"));
		}
	}
}
// SpawnNextPlantRPC ------------------------------------------------------------------

// DeathRPC ------------------------------------------------------------------
void AJS_SeedActor::Server_DeathRPC_Implementation()
{
	Multicast_DeathRPC();
}

void AJS_SeedActor::Multicast_DeathRPC_Implementation()
{
	this->Destroy();
}
// DeathRPC ------------------------------------------------------------------

