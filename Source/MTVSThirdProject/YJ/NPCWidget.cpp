// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UNPCWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	backButton->OnClicked.AddDynamic(this,&UNPCWidget::OnClickbackButton);
	frontButton->OnClicked.AddDynamic(this,&UNPCWidget::OnClickfrontButton);
}

void UNPCWidget::SetTextLog(FString str)
{

	// csv리더기의 한줄씩 읽어와서 표시하기
		// 다음 버튼을 클릭하면 알아서 다음 글이 나오도록 
	TextLog->SetText(FText::FromString(str));
}

void UNPCWidget::OnClickbackButton()
{
	if(0==curCount){return;}
	curCount--;
	// csv 리더기에서 str 받아온 다음 출력하기
	// 델리게이트에  curCount 만넘겨서 실행하기
	//+ 지금 현재 상호작용하는 npc의 QuestNum 도 생각을 해야함 
	ReadCSVDele.Broadcast(curCount);
}

void UNPCWidget::OnClickfrontButton()
{
	// 지금 현재 npc EachCSVLines 의 최대값보다 크면 return 해야함
	curCount++;
	// curCount 에 해당하는 줄글을 가져와서 표시하기
	ReadCSVDele.Broadcast(curCount);
}
