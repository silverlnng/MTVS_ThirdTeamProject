// Fill out your copyright notice in the Description page of Project Settings.


#include "JS_Gress.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AJS_Gress::AJS_Gress()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp->SetupAttachment(RootComponent);
	boxComp->SetBoxExtent(FVector(25));
	boxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	boxComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	boxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	gressMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("gressMeshComp"));
	gressMeshComp->SetupAttachment(boxComp);
	gressMeshComp->SetRelativeScale3D(FVector(0.5f));
	gressMeshComp->SetCollisionResponseToChannel(ECC_Visibility, ECollisionResponse::ECR_Ignore);

	ConstructorHelpers::FObjectFinder <UStaticMesh> gressMeshTemp(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (gressMeshTemp.Succeeded()) {
		gressMeshComp->SetStaticMesh(gressMeshTemp.Object);
	}
}

// Called when the game starts or when spawned
void AJS_Gress::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJS_Gress::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJS_Gress::GetDamage_Implementation(bool damage)
{
	if (damage) SetCurHP_Implementation(curHP - 1);
}

void AJS_Gress::SetCurHP_Implementation(float amount)
{
	if (amount <= 0) {
		Death_Implementation();
	}
	else curHP = amount;
}

void AJS_Gress::Death_Implementation()
{
	this->Destroy();
}

void AJS_Gress::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Overlap Detected in boxComp!"));

	// 디버그 메시지로 어떤 액터가 겹쳤는지 출력
	if (OtherActor) {
		FString actorName = OtherActor->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Overlapping with Actor: %s"), *actorName));
	}
}

