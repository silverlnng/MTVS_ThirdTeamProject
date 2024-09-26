// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCCharacter.h"

#include "AJH_Player.h"
#include "MovieSceneSequenceID.h"
#include "NPCWidget.h"
#include "YJHUD.h"
#include "YJPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/HorizontalBox.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	player = Cast<AAJH_Player>(OtherActor);
	//ATP_ThirdPersonCharacter* player_ = Cast<ATP_ThirdPersonCharacter>(OtherActor);
	if(player && player->IsLocallyControlled())
	{
		player->EnableInput(pc);
		pc->SetInputMode(FInputModeGameAndUI());
		NPC_UI->SetVisibility(ESlateVisibility::Hidden);
		NPC_UI->YesNoBox->SetVisibility(ESlateVisibility::Hidden);
		bisEndOverlap=true;
	}
}

void ANPCCharacter::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// OtherActor 가 플레이어 일때 그 플레이어의 로컬에게만
	player = Cast<AAJH_Player>(OtherActor);
	//ATP_ThirdPersonCharacter* player_ = Cast<ATP_ThirdPersonCharacter>(OtherActor);
	if(player && player->IsLocallyControlled()&&bisEndOverlap)
	{
		bisEndOverlap=false;
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
			player->DisableInput(pc);
			pc->SetInputMode(FInputModeUIOnly());
			pc->SetShowMouseCursor(true);
		}
		MakeEachCSVLines(QuestNum); // QuestNum 을 달성하면 올려야함
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
	FString CSVNPCInfoPath = FPaths::ProjectDir() / TEXT("NPCData.csv");
	FString CSVNPCGoalPath = FPaths::ProjectDir() / TEXT("NPCGoal.csv");

	// 파일 내용을 저장할 변수
	FString FileContent;
	FString FileContentGoal;
	// 파일 읽기
	if (FFileHelper::LoadFileToString(FileContent, *CSVNPCInfoPath))
	{
		FileContent.ParseIntoArrayLines(TotalCSVLines);
	}
	
	if (FFileHelper::LoadFileToString(FileContentGoal, *CSVNPCGoalPath))
	{
		FileContentGoal.ParseIntoArrayLines(GoalCSVLines);
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
	//EachCSVLines[start] 을 , 으로 나누고
	if (GoalCSVLines.Num() >= 0)
	{
		QuestGoal=GoalCSVLines[num-1];
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

