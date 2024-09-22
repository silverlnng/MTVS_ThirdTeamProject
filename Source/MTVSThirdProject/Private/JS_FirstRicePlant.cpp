// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_FirstRicePlant.h"
#include "JS_SecondRicePlant.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AJS_FirstRicePlant::AJS_FirstRicePlant()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50));
	boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	boxMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("boxMeshComp"));
	boxMeshComp->SetupAttachment(boxComp);
	boxMeshComp->SetRelativeScale3D(FVector(1));
	boxMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	ConstructorHelpers::FObjectFinder<UStaticMesh> boxMeshTemp(TEXT("/ Script / Engine.StaticMesh'/Engine/BasicShapes/Cube1.Cube1'"));
	if (boxMeshTemp.Succeeded()) {
		boxMeshComp->SetStaticMesh(boxMeshTemp.Object);
	}
	bReplicates = true;
}

void AJS_FirstRicePlant::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME (AJS_FirstRicePlant , PlantClassToSpawn);
	DOREPLIFETIME (AJS_FirstRicePlant , maxHP);
	DOREPLIFETIME (AJS_FirstRicePlant , curHP);
	DOREPLIFETIME (AJS_FirstRicePlant , checkDeltaTime);
	DOREPLIFETIME (AJS_FirstRicePlant , growTime);
}

// Called when the game starts or when spawned
void AJS_FirstRicePlant::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if ( playerController ) {
		SetOwner(playerController);
	}
}

// Called every frame
void AJS_FirstRicePlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	checkDeltaTime += DeltaTime;

	if(HasAuthority() && checkDeltaTime >= growTime){
		if (this->ActorHasTag(TEXT("Wheat")))
		{
			UE_LOG(LogTemp, Warning, TEXT("This actor has the Wheat tag."));
			SpawnNextPlant_Implementation(ricePlantID);
			GetDamage_Implementation(true);
		}
		if (this->ActorHasTag(TEXT("PumpKin"))) {
			UE_LOG(LogTemp, Warning, TEXT("This actor has the PumpKin tag."));
			SpawnNextPlant_Implementation(pumpKinPlantID);
			GetDamage_Implementation(true);
		}
		if (this->ActorHasTag(TEXT("Carrot"))) {
			UE_LOG(LogTemp, Warning, TEXT("This actor has the Carrot tag."));
			SpawnNextPlant_Implementation(carrotPlantID);
			GetDamage_Implementation(true);
		}
		if (this->ActorHasTag(TEXT("Strawberry"))) {
			UE_LOG(LogTemp, Warning, TEXT("This actor has the Strawberry tag."));
			SpawnNextPlant_Implementation(strawberryPlantID);
			GetDamage_Implementation(true);
		}
		if (this->ActorHasTag(TEXT("Watermelon"))) {
			UE_LOG(LogTemp, Warning, TEXT("This actor has the Watermelon tag."));
			SpawnNextPlant_Implementation(watermelonPlantID);
			GetDamage_Implementation(true);
		}
	}
}

void AJS_FirstRicePlant::GetDamage_Implementation(bool damage)
{
	Server_GetDamageRPC(damage);
}

void AJS_FirstRicePlant::SetCurHP_Implementation(float amount)
{
	Server_SetCurHPRPC(amount);
}

void AJS_FirstRicePlant::SpawnNextPlant_Implementation(int32 index)
{
	if (HasAuthority()) Server_SpawnNextPlantRPC(index);
}

void AJS_FirstRicePlant::Death_Implementation()
{
	Server_DeathRPC();
}

// GetDamageRPC ------------------------------------------------------------------
void AJS_FirstRicePlant::Server_GetDamageRPC_Implementation(bool damage)
{
	Multicast_GetDamageRPC(damage);
}
void AJS_FirstRicePlant::Multicast_GetDamageRPC_Implementation(bool damage)
{
	if ( damage ) SetCurHP_Implementation(curHP - 1);
}
// GetDamageRPC ------------------------------------------------------------------

// SetCurHPRPC ------------------------------------------------------------------
void AJS_FirstRicePlant::Server_SetCurHPRPC_Implementation(float amount)
{
	Multicast_SetCurHPRPC(amount);
}
void AJS_FirstRicePlant::Multicast_SetCurHPRPC_Implementation(float amount)
{
	if ( amount <= 0 ) Death_Implementation();
	else curHP = amount;
}
// SetCurHPRPC ------------------------------------------------------------------

// SpawnNextPlantRPC ------------------------------------------------------------------
void AJS_FirstRicePlant::Server_SpawnNextPlantRPC_Implementation(int32 index)
{
	if ( HasAuthority() ) Multicast_SpawnNextPlantRPC(index);
}
void AJS_FirstRicePlant::Multicast_SpawnNextPlantRPC_Implementation(int32 index)
{
	if ( HasAuthority() ) {
		switch ( index )
		{
		case 11210:
			PlantClassToSpawn = SpawnSecondRicePlant;
			UE_LOG(LogTemp , Warning , TEXT("id : 11210 SecondRicePlant spawn"));
			break;
		case 11211:
			PlantClassToSpawn = SpawnSecondWatermelonPlant;
			UE_LOG(LogTemp , Warning , TEXT("id : 11211 SecondWatermelonPlant spawn"));
			break;
		case 11212:
			PlantClassToSpawn = SpawnSecondStrawberryPlant;
			UE_LOG(LogTemp , Warning , TEXT("id : 11212 SecondStrawberryPlant spawn"));
			break;
		case 11213:
			PlantClassToSpawn = SpawnSecondCarrotPlant;
			UE_LOG(LogTemp , Warning , TEXT("id : 11213 SecondCarrotPlant spawn"));
			break;
		case 11214:
			PlantClassToSpawn = SpawnSecondPumpkinPlant;
			UE_LOG(LogTemp , Warning , TEXT("id : 11214 SecondPumpkinPlant spawn"));
			break;

		default:
			break;
		}

		if ( PlantClassToSpawn ) {
			//선택된 블루프린트 클래스로 액터 스폰
			AJS_SecondRicePlant* SpawnPlant = GetWorld()->SpawnActor<AJS_SecondRicePlant>(PlantClassToSpawn , GetActorLocation() , FRotator::ZeroRotator);
			if ( SpawnPlant ) {
				SpawnPlant;
			}
			else UE_LOG(LogTemp , Warning , TEXT("Failed to spawn AJS_FirstRicePlant"));
		}
	}
}
// SpawnNextPlantRPC ------------------------------------------------------------------

// DeathRPC ------------------------------------------------------------------
void AJS_FirstRicePlant::Server_DeathRPC_Implementation()
{
	Multicast_DeathRPC();
}

void AJS_FirstRicePlant::Multicast_DeathRPC_Implementation()
{
	this->Destroy();
}
// DeathRPC ------------------------------------------------------------------




