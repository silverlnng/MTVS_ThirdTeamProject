// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_ObstacleActor.h"
#include "Components/BoxComponent.h"

// Sets default values
AJS_ObstacleActor::AJS_ObstacleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(RootComponent);
	boxComp->SetBoxExtent(FVector(50));
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	boxComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	boxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	staticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TreeMeshComp"));
	staticMeshComp->SetupAttachment(boxComp);
	staticMeshComp->SetRelativeScale3D(FVector(1));
	staticMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	ConstructorHelpers::FObjectFinder <UStaticMesh> staticMeshTemp(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (staticMeshTemp.Succeeded()) {
		staticMeshComp->SetStaticMesh(staticMeshTemp.Object);
	}
}

// Called when the game starts or when spawned
void AJS_ObstacleActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJS_ObstacleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJS_ObstacleActor::GetDamage_Implementation(bool damage)
{
	if (damage) {
		if (this->ActorHasTag(TEXT("Tree"))) {
			btree = true;
			SetCurHP_Implementation(treeCurHP - 1);

		}
		else if (this->ActorHasTag(TEXT("Rock")) || this->ActorHasTag(TEXT("Gress"))){
			SetCurHP_Implementation(curHP - 5);
		}
	}
	
}

void AJS_ObstacleActor::SetCurHP_Implementation(float amount)
{
	if (amount <= 0) {
		Death_Implementation();
	}
	if(btree){
		btree = false;
		treeCurHP = amount;
	}
	else curHP = amount;
}

void AJS_ObstacleActor::Death_Implementation()
{
	this->Destroy();
}

void AJS_ObstacleActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Overlap Detected in boxComp!"));

	// 디버그 메시지로 어떤 액터가 겹쳤는지 출력
	if (OtherActor) {
		FString actorName = OtherActor->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Overlapping with Actor: %s"), *actorName));
	}

}

