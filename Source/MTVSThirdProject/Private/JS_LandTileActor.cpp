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
	boxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	landTileComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("landTileComp"));
	landTileComp->SetupAttachment(boxComp);
	landTileComp->SetRelativeScale3D(FVector(1, 1, 0.01f));
	

	ConstructorHelpers::FObjectFinder<UStaticMesh> landTileMeshTemp(TEXT("/ Script / Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	if (landTileMeshTemp.Succeeded()) {
		landTileComp->SetStaticMesh(landTileMeshTemp.Object);
	}
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
	
	//틱에서 레이를 맞았는지 계속 체크해서 
	//플레이어가 상호작용을 했다면
			//GetDamager(true);
	//if (Tile.occupyingActor->ActorHasTag("Tree")) {
	//	//GetDamager(true);
	//}
	//else if (Tile.occupyingActor->ActorHasTag("Gress")) {
	//	//GetDamager(true);
	//}
	//else if (Tile.occupyingActor->ActorHasTag("Rock")) {
	//	//GetDamager(true);
	//}
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

	// 디버그 메시지로 어떤 액터가 겹쳤는지 출력
	if (OtherActor) {
	FString actorName = OtherActor->GetName();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Overlapping with Actor: %s"), *actorName));
	}
}
