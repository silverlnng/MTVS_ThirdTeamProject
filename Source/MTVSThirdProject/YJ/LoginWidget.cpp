// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"

#include "NetWorkGameInstance.h"
#include "SessionSlotWidget.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Components/ScrollBox.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();
	btn_create->OnClicked.AddDynamic(this,&ULoginWidget::OnClickedCreateButton);
	gi = GetGameInstance<UNetWorkGameInstance>();//게임프레임워크는 가져오기 쉬움

	if (gi != nullptr)
	{
		// 세션 찾기 델리게이트에 함수를 연결한다.
		gi->onCreateSlot.AddDynamic(this, &ULoginWidget::OnSlotCreated);
		/*gi->onNewSearchComplete.AddDynamic(this, &ULoginWidget::OnClearScrollBox);
		gi->onFindButtonToggle.AddDynamic(this, &ULoginWidget::FindButtonOnOff);*/
		btn_findSessions->OnClicked.AddDynamic(this, &ULoginWidget::OnClickedFindSessionsButton);
	}
}

void ULoginWidget::OnClickedCreateButton()
{
	if(gi!=nullptr)
	{
		gi->SetSessionName(eText_hostName->GetText().ToString());
		gi->CreateMySession(eText_roomName->GetText().ToString(), eText_hostName->GetText().ToString(), 7);
	}
}

void ULoginWidget::OnSlotCreated(FString roomName, FString hostName, int32 currentPlayers, int32 maxPlayers, int32 ping, int32 sessionIdx)
{
	// 서버로부터 받은 정보로 슬롯 위젯을 만들어서 추가한다.
	if (slotWidget != nullptr)
	{
		if (USessionSlotWidget* slot_UI = CreateWidget<USessionSlotWidget>(GetWorld(), slotWidget))
		{
			slot_UI->SetSessionInfo(roomName, hostName, currentPlayers, maxPlayers, ping, sessionIdx);
			sb_roomList->AddChild(slot_UI);
		}
	}
}

void ULoginWidget::OnClickedFindSessionsButton()
{
	if (gi != nullptr)
	{
		gi->FindMySession();
	}
}
