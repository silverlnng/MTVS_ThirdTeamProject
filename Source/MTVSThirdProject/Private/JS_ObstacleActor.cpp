// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_ObstacleActor.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

// Sets default values
AJS_ObstacleActor::AJS_ObstacleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50));
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	boxComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	boxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	staticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TreeMeshComp"));
	staticMeshComp->SetupAttachment(boxComp);
	staticMeshComp->SetRelativeScale3D(FVector(1));
	staticMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	SetSoundfile();

	bReplicates = true;
}

void AJS_ObstacleActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//변수에 replicated를 사용한다면 등록해야함.
	DOREPLIFETIME (AJS_ObstacleActor , btree);
	DOREPLIFETIME (AJS_ObstacleActor , treeMaxHP);
	DOREPLIFETIME (AJS_ObstacleActor , treeCurHP);
	DOREPLIFETIME (AJS_ObstacleActor , maxHP);
	DOREPLIFETIME (AJS_ObstacleActor , curHP);
}

// Called when the game starts or when spawned
void AJS_ObstacleActor::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	if ( playerController ) {
		SetOwner(playerController);
	}
}

// Called every frame
void AJS_ObstacleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJS_ObstacleActor::GetDamage_Implementation(bool damage)
{
	Server_GetDamage(damage);
}

void AJS_ObstacleActor::SetCurHP_Implementation(float amount)
{
	Server_SetCurHP(amount);
}

void AJS_ObstacleActor::Death_Implementation()
{
	Server_Death();
}

void AJS_ObstacleActor::Server_GetDamage_Implementation(bool damage)
{
	Multicast_GetDamage(damage);
}

void AJS_ObstacleActor::Multicast_GetDamage_Implementation(bool damage)
{
	if ( damage ) {
		if ( this->ActorHasTag(TEXT("Tree")) ) {
			btree = true;
			UGameplayStatics::PlaySoundAtLocation(this, treeGetDamageSound, GetActorLocation());
			PlayObstacleSound();
			SetCurHP_Implementation(treeCurHP - 1);

		}
		else if ( this->ActorHasTag(TEXT("Rock")) || this->ActorHasTag(TEXT("Gress")) ) {
			PlayObstacleSound();
			SetCurHP_Implementation(curHP - 5);
		}
	}
}

void AJS_ObstacleActor::Server_SetCurHP_Implementation(float amount)
{
	Multicast_SetCurHP(amount);
}

void AJS_ObstacleActor::Multicast_SetCurHP_Implementation(float amount)
{
	if ( amount <= 0 ) {
		Death_Implementation();
	}
	if ( btree ) {
		btree = false;
		treeCurHP = amount;
	}
	else curHP = amount;
}

void AJS_ObstacleActor::Server_Death_Implementation()
{
	Multicast_Death();
}

void AJS_ObstacleActor::Multicast_Death_Implementation()
{
	this->Destroy();
}

void AJS_ObstacleActor::PlayObstacleSound()
{
	Server_PlayObstacleSound();
}

void AJS_ObstacleActor::Server_PlayObstacleSound_Implementation()
{
	Multicast_PlayObstacleSound();
}

void AJS_ObstacleActor::Multicast_PlayObstacleSound_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("PlayObstacleSound IN"));
	if (treeCurHP <= 0 && this->ActorHasTag(TEXT("Tree")) ) {
		int32 playNumber = FMath::RandRange(0 , 3);

		switch ( playNumber ) {
		case 0:
			UGameplayStatics::PlaySoundAtLocation(this , treeDistroySound1 , GetActorLocation());
			break;
		case 1:
			UGameplayStatics::PlaySoundAtLocation(this , treeDistroySound2 , GetActorLocation());
			break;
		case 2:
			UGameplayStatics::PlaySoundAtLocation(this , treeDistroySound3 , GetActorLocation());
			break;
		case 3:
			UGameplayStatics::PlaySoundAtLocation(this , treeDistroySound4 , GetActorLocation());
			break;
		default:
			break;
		}
	}
	if ( this->ActorHasTag(TEXT("Rock")) ) {
		int32 playNumber = FMath::RandRange(0 , 3);

		switch ( playNumber ) {
		case 0:
			UGameplayStatics::PlaySoundAtLocation(this , rockDistroySound1 , GetActorLocation());
			break;
		case 1:
			UGameplayStatics::PlaySoundAtLocation(this , rockDistroySound2 , GetActorLocation());
			break;
		case 2:
			UGameplayStatics::PlaySoundAtLocation(this , rockDistroySound3 , GetActorLocation());
			break;
		case 3:
			UGameplayStatics::PlaySoundAtLocation(this , rockDistroySound4 , GetActorLocation());
			break;
		default:
			break;
		}
	}
	if ( this->ActorHasTag(TEXT("Gress")) ) {
		int32 playNumber = FMath::RandRange(0 , 3);

		switch ( playNumber ) {
		case 0:
			UGameplayStatics::PlaySoundAtLocation(this , gressDistroySound1, GetActorLocation());
			break;
		case 1:
			UGameplayStatics::PlaySoundAtLocation(this , gressDistroySound2, GetActorLocation());
			break;
		case 2:
			UGameplayStatics::PlaySoundAtLocation(this , gressDistroySound3, GetActorLocation());
			break;
		case 3:
			UGameplayStatics::PlaySoundAtLocation(this , gressDistroySound4, GetActorLocation());
			break;
		default:
			break;
		}
	}
}
void AJS_ObstacleActor::SetSoundfile()
{
	//Tree Sound
	ConstructorHelpers::FObjectFinder<USoundBase> treeDistroySoundTemp1(TEXT("/Script/Engine.SoundWave'/Game/JS/Sound/Tree/MinCraft_TreeDistroySouind1.MinCraft_TreeDistroySouind1'"));
	if ( treeDistroySoundTemp1.Succeeded() ) treeDistroySound1 = treeDistroySoundTemp1.Object;
	ConstructorHelpers::FObjectFinder<USoundBase> treeDistroySoundTemp2(TEXT("/Script/Engine.SoundWave'/Game/JS/Sound/Tree/MinCraft_TreeDistroySouind2.MinCraft_TreeDistroySouind2'"));
	if ( treeDistroySoundTemp2.Succeeded() ) treeDistroySound2 = treeDistroySoundTemp2.Object;
	ConstructorHelpers::FObjectFinder<USoundBase> treeDistroySoundTemp3(TEXT("/Script/Engine.SoundWave'/Game/JS/Sound/Tree/MinCraft_TreeDistroySouind3.MinCraft_TreeDistroySouind3'"));
	if ( treeDistroySoundTemp3.Succeeded() ) treeDistroySound3 = treeDistroySoundTemp3.Object;
	ConstructorHelpers::FObjectFinder<USoundBase> treeDistroySoundTemp4(TEXT("/Script/Engine.SoundWave'/Game/JS/Sound/Tree/MinCraft_TreeDistroySouind4.MinCraft_TreeDistroySouind4'"));
	if ( treeDistroySoundTemp4.Succeeded() ) treeDistroySound4 = treeDistroySoundTemp4.Object;
	ConstructorHelpers::FObjectFinder<USoundBase> treeGetDamageSoundTemp(TEXT("/Script/Engine.SoundWave'/Game/JS/Sound/Tree/MinCraft_TreeAttackSound.MinCraft_TreeAttackSound'"));
	if ( treeGetDamageSoundTemp.Succeeded() ) treeGetDamageSound = treeGetDamageSoundTemp.Object;
	//Tree Sound

	//Rock Sound
	ConstructorHelpers::FObjectFinder<USoundBase> rockDistroySoundTemp1(TEXT("/Script/Engine.SoundWave'/Game/JS/Sound/Rock/MinCraft_RockSound1.MinCraft_RockSound1'"));
	if ( rockDistroySoundTemp1.Succeeded() ) rockDistroySound1 = rockDistroySoundTemp1.Object;
	ConstructorHelpers::FObjectFinder<USoundBase> rockDistroySoundTemp2(TEXT("/Script/Engine.SoundWave'/Game/JS/Sound/Rock/MinCraft_RockSound2.MinCraft_RockSound2'"));
	if ( rockDistroySoundTemp2.Succeeded() ) rockDistroySound2 = rockDistroySoundTemp2.Object;
	ConstructorHelpers::FObjectFinder<USoundBase> rockDistroySoundTemp3(TEXT("/Script/Engine.SoundWave'/Game/JS/Sound/Rock/MinCraft_RockSound3.MinCraft_RockSound3'"));
	if ( rockDistroySoundTemp3.Succeeded() ) rockDistroySound3 = rockDistroySoundTemp3.Object;
	ConstructorHelpers::FObjectFinder<USoundBase> rockDistroySoundTemp4(TEXT("/Script/Engine.SoundWave'/Game/JS/Sound/Rock/MinCraft_RockSound4.MinCraft_RockSound4'"));
	if ( rockDistroySoundTemp4.Succeeded() ) rockDistroySound4 = rockDistroySoundTemp4.Object;
	//Rock Sound

	//Gress Sound
	ConstructorHelpers::FObjectFinder<USoundBase> gressDistroySound1Temp(TEXT("/Script/Engine.SoundWave'/Game/JS/Sound/Gress/MinCraft_GressSound1.MinCraft_GressSound1'"));
	if ( gressDistroySound1Temp.Succeeded() ) gressDistroySound1 = gressDistroySound1Temp.Object;

	ConstructorHelpers::FObjectFinder<USoundBase> gressDistroySound2Temp(TEXT("/Script/Engine.SoundWave'/Game/JS/Sound/Gress/MinCraft_GressSound2.MinCraft_GressSound2'"));
	if ( gressDistroySound2Temp.Succeeded() ) gressDistroySound2 = gressDistroySound2Temp.Object;

	ConstructorHelpers::FObjectFinder<USoundBase> gressDistroySound3Temp(TEXT("/Script/Engine.SoundWave'/Game/JS/Sound/Gress/MinCraft_GressSound3.MinCraft_GressSound3'"));
	if ( gressDistroySound3Temp.Succeeded() ) gressDistroySound3 = gressDistroySound3Temp.Object;

	ConstructorHelpers::FObjectFinder<USoundBase> gressDistroySound4Temp(TEXT("/Script/Engine.SoundWave'/Game/JS/Sound/Gress/MinCraft_GressSound4.MinCraft_GressSound4'"));
	if ( gressDistroySound4Temp.Succeeded() ) gressDistroySound4 = gressDistroySound4Temp.Object;
	// Gress Sound
}

void AJS_ObstacleActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep , const FHitResult& SweepResult)
{

}