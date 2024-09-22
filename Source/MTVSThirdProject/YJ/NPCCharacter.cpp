// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacter.h"

#include "AJH_Player.h"
#include "MovieSceneSequenceID.h"
#include "NPCWidget.h"
#include "YJHUD.h"
#include "YJPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/HorizontalBox.h"
#include "MTVSThirdProject/TP_ThirdPerson/TP_ThirdPersonCharacter.h"

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
	BoxComp->OnComponentEndOverlap.AddDynamic(this,&ANPCCharacter::OnBoxEndOverlap);
	
	pc= Cast<AYJPlayerController>(GetWorld()->GetFirstPlayerController());
	if (pc)
	{
		MyHUD = pc->GetHUD<AYJHUD>();
		if (MyHUD)
		{
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, [this]()
			{
				if (MyHUD->NPCUI)
				{
					NPC_UI = MyHUD->NPCUI;
					//NPC_UI->ReadCSVDele.AddDynamic(this,&ANPCCharacter::ReadEachLinesNum);
					//CSVLinesNum(0);
				}
			}, 1.0f, false);
		}
	}
	
	ReadCSV();
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, [this]()
	{
		MakeEachCSVLines(QuestNum);
	}, 1.0f, false);
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

void ANPCCharacter::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	pc->SetInputMode(FInputModeGameOnly());
	pc->SetShowMouseCursor(false);
	NPC_UI->SetVisibility(ESlateVisibility::Hidden);
	NPC_UI->YesNoBox->SetVisibility(ESlateVisibility::Hidden);
}

void ANPCCharacter::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// OtherActor 가 플레이어 일때 그 플레이어의 로컬에게만
	//AAJH_Player* player = Cast<AAJH_Player>(OtherActor);
	ATP_ThirdPersonCharacter* player = Cast<ATP_ThirdPersonCharacter>(OtherActor);
	if(player && player->IsLocallyControlled())
	{
		pc = player->GetController<AYJPlayerController>();
		if (pc)
		{
			MyHUD = pc->GetHUD<AYJHUD>();
			pc->CurNPC = this;
			if (MyHUD)
			{
				if (MyHUD->NPCUI)
				{
					NPC_UI=MyHUD->NPCUI;
					NPC_UI->SetVisibility(ESlateVisibility::Visible);
					NPC_UI->curCount = 0;
				}
			}
			pc->SetInputMode(FInputModeUIOnly());
			pc->SetShowMouseCursor(true);
		}
		
		// csvmanager 를 통해 읽어오기, 
		ReadEachLinesNum(0);
		// NPCUI 가 나오도록 하고 처음 부터 나오도록 하기
	}
	
	// 일단 ui 나오도록 ui 의 소유권은 누가 ?

	// ACSVManager::MakeEachCSVLines(int32 num) 에 자신의 QuestNum 를 전달하고 실행
	// npc가 어떻게 ACSVManager를 알게 할것인지 ?? 
	
}

void ANPCCharacter::ReadCSV()
{
	FString CSVPath = FPaths::ProjectDir() / TEXT("NPCData.csv");

	// 파일 내용을 저장할 변수
	FString FileContent;
	
	// 파일 읽기
	if (FFileHelper::LoadFileToString(FileContent, *CSVPath))
	{
		FileContent.ParseIntoArrayLines(TotalCSVLines);
	}	
}

void ANPCCharacter::MakeEachCSVLines(int32 num)
{
	EachCSVLines.Empty();
	// TotalCSVLines 에서 현재 퀘스트 넘버를 적용한 EachCSVLines 를 만들기
	int32 start;
	TotalCSVLines.Find(FString::FromInt(num),start);
	
	int32 end;
	TotalCSVLines.Find(FString::FromInt(num+1),end);
	
	for (int32 i = start+1; i < end; ++i)
	{
		EachCSVLines.Add(TotalCSVLines[i]);
	}
}

void ANPCCharacter::ReadEachLinesNum(int32 num)
{
	// 이때 num 이 EachCSVLines 보다 크면 x => 수락,거절 ui
	if(num>=EachCSVLines.Num())
	{
		NPC_UI->YesNoBox->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	FString str = EachCSVLines[num];
	//이걸 ui 에 나오게 해야함
	NPC_UI->SetTextLog(str);
}

