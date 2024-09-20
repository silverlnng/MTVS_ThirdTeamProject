// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacter.h"

#include "AJH_Player.h"
#include "MovieSceneSequenceID.h"
#include "NPCWidget.h"
#include "YJHUD.h"
#include "Components/BoxComponent.h"

// Sets default values
ANPCCharacter::ANPCCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComp =CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this,&ANPCCharacter::OnBoxBeginOverlap);
}

// Called every frame
void ANPCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANPCCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ANPCCharacter::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// OtherActor 가 플레이어 일때 그 플레이어의 로컬에게만
	if(OtherActor->IsA(AAJH_Player::StaticClass()))
	{
		if(GetController() && GetController()->IsLocalController())
		{
			auto* pc =GetController<APlayerController>();
			MyHUD=pc->GetHUD<AYJHUD>();
			// csvmanager 를 통해 읽어오기
			MyHUD->NPCUI->SetVisibility(ESlateVisibility::Visible);
		}
	}
	
	// 일단 ui 나오도록 ui 의 소유권은 누가 ?

	// ACSVManager::MakeEachCSVLines(int32 num) 에 자신의 QuestNum 를 전달하고 실행
	// npc가 어떻게 ACSVManager를 알게 할것인지 ?? 
	
}

