// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginFirstWidget.h"

#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableText.h"
#include "Components/HorizontalBox.h"
#include "NetWorkGameInstance.h"

void ULoginFirstWidget::NativeConstruct()
{
	Super::NativeConstruct();
	gi = GetGameInstance<UNetWorkGameInstance>();
	btn_findSessions->OnClicked.AddDynamic(this, &ULoginFirstWidget::OnClickedFindSessionsButton);
	ResetButton->OnClicked.AddDynamic(this, &ULoginFirstWidget::OnClickedResetButton);
	ComboBoxStr_Char->OnSelectionChanged.AddDynamic(this,&ULoginFirstWidget::SelectCharacter);
	HBox_char->SetVisibility(ESlateVisibility::Hidden);
}

void ULoginFirstWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void ULoginFirstWidget::OnClickedFindSessionsButton()
{
	if (gi != nullptr)
	{
		// 누를때 닉네임과 캐릭터 선택값을 저장하기
		gi->UserNickName=eText_hostName->GetText().ToString();
		gi->FindMySession();
	}
}

void ULoginFirstWidget::OnClickedResetButton()
{
	// 지금현재있는 방들 삭제 하고
	// 내가 속해있는 세션도 나가고
	gi->ExitMySession();
	// 룸만들기
	//gi->CreateMySession();
	gi->OnDestroyAllSessions();
}

void ULoginFirstWidget::SelectCharacter(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (gi != nullptr)
	{
		if (SelectedItem.Contains(FString("토끼")))
		{
			gi->meshNum = 0;
		}
		else if (SelectedItem.Equals(FString("판다")))
		{
			gi->meshNum = 1;
		}
		else if (SelectedItem == FString("족제비"))
		{
			gi->meshNum = 2;
		}

		UE_LOG(LogTemp, Warning, TEXT("User Selected Mesh: %s"), *SelectedItem);
	}
}
