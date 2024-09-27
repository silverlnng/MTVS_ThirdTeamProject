// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_SecondRicePlant.h"
#include "Components/SphereComponent.h"
#include "JS_Rice.h"
#include "JS_Pumpkin.h"
#include "JS_Pumpkin.h"
#include "JS_Watermelon.h"
#include "JS_Carrot.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"


// Sets default values
AJS_SecondRicePlant::AJS_SecondRicePlant()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
	SetRootComponent(sphereComp);
	sphereComp->SetRelativeScale3D(FVector(1));
	sphereComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	sphereMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sphereMeshComp"));
	sphereMeshComp->SetupAttachment(sphereComp);
	sphereMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	ConstructorHelpers::FObjectFinder<UStaticMesh> sphereMeshTemp(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (sphereMeshTemp.Succeeded()) {
		sphereMeshComp->SetStaticMesh(sphereMeshTemp.Object);
	}

	ConstructorHelpers::FObjectFinder<USoundBase> getActorSoundTemp(TEXT("/Script/Engine.SoundWave'/Game/JS/Sound/InteractSound/GetSound.GetSound'"));
	if ( getActorSoundTemp.Succeeded() ) getActorSound = getActorSoundTemp.Object;

	ConstructorHelpers::FObjectFinder<USoundBase> grownPlantSoundTemp(TEXT("/Script/Engine.SoundWave'/Game/JS/Sound/Plant/Mincraft_GrownPlant.Mincraft_GrownPlant'"));
	if ( grownPlantSoundTemp.Succeeded() ) grownPlantSound = grownPlantSoundTemp.Object;

	bReplicates = true;
}

void AJS_SecondRicePlant::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME (AJS_SecondRicePlant , PlantClassToSpawn);
	DOREPLIFETIME (AJS_SecondRicePlant , maxHP);
	DOREPLIFETIME (AJS_SecondRicePlant , curHP);
	//수확할 때 퀘스트에서 증가할 카운트
	DOREPLIFETIME (AJS_SecondRicePlant , harvestWheatCount);
	DOREPLIFETIME (AJS_SecondRicePlant , harvestPumpkinCount);
	DOREPLIFETIME (AJS_SecondRicePlant , harvestCarrotCount);
	DOREPLIFETIME (AJS_SecondRicePlant , harvestStrawberryCount);
	DOREPLIFETIME (AJS_SecondRicePlant , harvestWatermelonCount);
}

// Called when the game starts or when spawned
void AJS_SecondRicePlant::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if ( playerController ) {
		SetOwner(playerController);
	}
	UGameplayStatics::PlaySoundAtLocation(this , grownPlantSound , GetActorLocation());
}

// Called every frame
void AJS_SecondRicePlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() && bInteractSecondPlant) {
		if (this->ActorHasTag(TEXT("Wheat")))
		{
			if( harvestWheatCount < 4) harvestWheatCount++;
			UE_LOG(LogTemp, Warning, TEXT("This actor has the Wheat tag."));
			SpawnNextPlant_Implementation(riceID);
			GetDamage_Implementation(true);
		}
		else if (this->ActorHasTag(TEXT("PumpKin"))) {
			 if ( harvestPumpkinCount < 4 ) harvestPumpkinCount++;
			 UE_LOG(LogTemp, Warning, TEXT("This actor has the PumpKin tag."));
			 SpawnNextPlant_Implementation(pumpKinID);
			 GetDamage_Implementation(true);
		}
		else if (this->ActorHasTag(TEXT("Carrot"))) {
			 if ( harvestCarrotCount < 4 ) harvestCarrotCount++;
			 UE_LOG(LogTemp, Warning, TEXT("This actor has the Carrot tag."));
			 SpawnNextPlant_Implementation(carrotID);
			 GetDamage_Implementation(true);
		}
		else if (this->ActorHasTag(TEXT("Strawberry"))) {
			if ( harvestStrawberryCount < 4 ) harvestStrawberryCount++;
			UE_LOG(LogTemp, Warning, TEXT("This actor has the Strawberry tag."));
			SpawnNextPlant_Implementation(strawberryID);
			GetDamage_Implementation(true);
		}
		else if (this->ActorHasTag(TEXT("Watermelon"))) {
			if ( harvestWatermelonCount < 4 ) harvestWatermelonCount++;
			UE_LOG(LogTemp, Warning, TEXT("This actor has the Watermelon tag."));
			SpawnNextPlant_Implementation(watermelonID);
			GetDamage_Implementation(true);
		}
	}
}

void AJS_SecondRicePlant::GetDamage_Implementation(bool damage)
{
	Server_GetDamageRPC(damage);
}

void AJS_SecondRicePlant::SetCurHP_Implementation(float amount)
{
	Server_SetCurHPRPC(amount);
}

void AJS_SecondRicePlant::SpawnNextPlant_Implementation(int32 index)
{
	Server_SpawnNextPlantRPC(index);
}

void AJS_SecondRicePlant::Death_Implementation()
{
	Server_DeathRPC();
}

// GetDamageRPC ------------------------------------------------------------------
void AJS_SecondRicePlant::Server_GetDamageRPC_Implementation(bool damage)
{
	Multicast_GetDamageRPC(damage);
}
void AJS_SecondRicePlant::Multicast_GetDamageRPC_Implementation(bool damage)
{
	if ( damage ) SetCurHP_Implementation(curHP - 1);
}
// GetDamageRPC ------------------------------------------------------------------

// SetCurHPRPC ------------------------------------------------------------------
void AJS_SecondRicePlant::Server_SetCurHPRPC_Implementation(float amount)
{
	Multicast_SetCurHPRPC(amount);
}
void AJS_SecondRicePlant::Multicast_SetCurHPRPC_Implementation(float amount)
{
	if ( amount <= 0 ) Death_Implementation();
	else curHP = amount;
}
// SetCurHPRPC ------------------------------------------------------------------

// SpawnNextPlantRPC ------------------------------------------------------------------
void AJS_SecondRicePlant::Server_SpawnNextPlantRPC_Implementation(int32 index)
{
	Multicast_SpawnNextPlantRPC(index);
}
void AJS_SecondRicePlant::Multicast_SpawnNextPlantRPC_Implementation(int32 index)
{
	if ( HasAuthority() ) {
		switch ( index )
		{
		case 70100:
			PlantClassToSpawn = RiceFactory;
			UE_LOG(LogTemp , Warning , TEXT("id : 70100 RiceFactory spawn"));
			break;
		case 70101:
			PlantClassToSpawn = WatermelonFactory;
			UE_LOG(LogTemp , Warning , TEXT("id : 70101 WatermelonFactory spawn"));
			break;
		case 70102:
			PlantClassToSpawn = StrawberryFactory;
			UE_LOG(LogTemp , Warning , TEXT("id : 70102 StrawberryFactory spawn"));
			break;
		case 70104:
			PlantClassToSpawn = CarrotFactory;
			UE_LOG(LogTemp , Warning , TEXT("id : 70104 CarrotFactory spawn"));
			break;
		case 70105:
			PlantClassToSpawn = PumpkinFactory;
			UE_LOG(LogTemp , Warning , TEXT("id : 70105 PumpkinFactory spawn"));
			break;
		default:
			break;
		}

		if ( PlantClassToSpawn && this->ActorHasTag(TEXT("Wheat")) ) {
			//선택된 블루프린트 클래스로 액터 스폰
			AJS_Rice* SpawnPlant = GetWorld()->SpawnActor<AJS_Rice>(PlantClassToSpawn , GetActorLocation() , FRotator::ZeroRotator);
			if ( SpawnPlant ) {
				UGameplayStatics::PlaySoundAtLocation(this, getActorSound, GetActorLocation());
				SpawnPlant;
			}
			else UE_LOG(LogTemp , Warning , TEXT("Failed to spawn SpawnPlant"));
		}
		if ( PlantClassToSpawn && this->ActorHasTag(TEXT("Watermelon")) ) {
			//선택된 블루프린트 클래스로 액터 스폰
			AJS_Watermelon* SpawnPlant = GetWorld()->SpawnActor<AJS_Watermelon>(PlantClassToSpawn , GetActorLocation() , FRotator::ZeroRotator);
			UGameplayStatics::PlaySoundAtLocation(this , getActorSound , GetActorLocation());
			if ( SpawnPlant ) {
				SpawnPlant;
			}
			else UE_LOG(LogTemp , Warning , TEXT("Failed to spawn SpawnPlant"));
		}
		if ( PlantClassToSpawn && this->ActorHasTag(TEXT("Strawberry")) ) {
			//선택된 블루프린트 클래스로 액터 스폰
			AJS_Strawberry* SpawnPlant = GetWorld()->SpawnActor<AJS_Strawberry>(PlantClassToSpawn , GetActorLocation() , FRotator::ZeroRotator);
			UGameplayStatics::PlaySoundAtLocation(this , getActorSound , GetActorLocation());
			if ( SpawnPlant ) {
				SpawnPlant;
			}
			else UE_LOG(LogTemp , Warning , TEXT("Failed to spawn SpawnPlant"));
		}
		if ( PlantClassToSpawn && this->ActorHasTag(TEXT("Pumpkin")) ) {
			//선택된 블루프린트 클래스로 액터 스폰
			AJS_Pumpkin* SpawnPlant = GetWorld()->SpawnActor<AJS_Pumpkin>(PlantClassToSpawn , GetActorLocation() , FRotator::ZeroRotator);
			UGameplayStatics::PlaySoundAtLocation(this , getActorSound , GetActorLocation());
			if ( SpawnPlant ) {
				SpawnPlant;
			}
			else UE_LOG(LogTemp , Warning , TEXT("Failed to spawn SpawnPlant"));
		}
		if ( PlantClassToSpawn && this->ActorHasTag(TEXT("Carrot")) ) {
			//선택된 블루프린트 클래스로 액터 스폰
			AJS_Carrot* SpawnPlant = GetWorld()->SpawnActor<AJS_Carrot>(PlantClassToSpawn , GetActorLocation() , FRotator::ZeroRotator);
			UGameplayStatics::PlaySoundAtLocation(this , getActorSound , GetActorLocation());
			if ( SpawnPlant ) {
				SpawnPlant;
			}
			else UE_LOG(LogTemp , Warning , TEXT("Failed to spawn SpawnPlant"));
		}
	}
}
// SpawnNextPlantRPC ------------------------------------------------------------------

// DeathRPC ------------------------------------------------------------------
void AJS_SecondRicePlant::Server_DeathRPC_Implementation()
{
	Multicast_DeathRPC();
}

void AJS_SecondRicePlant::Multicast_DeathRPC_Implementation()
{
	this->Destroy();
}
// DeathRPC ------------------------------------------------------------------



