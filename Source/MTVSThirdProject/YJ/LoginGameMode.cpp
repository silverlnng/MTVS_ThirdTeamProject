// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGameMode.h"
#include "LoginFirstWidget.h"
#include "OnlineSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableText.h"
#include "ThirdParty/Steamworks/Steamv157/sdk/public/steam/steam_api.h"

#pragma comment(lib, "ThirdParty/Steamworks/Steamv157/sdk/redistributable_bin/win64/steam_api64.lib")

void ALoginGameMode::BeginPlay()
{
	Super::BeginPlay();
	if(loginWidget != nullptr)
	{
		loginUI = CreateWidget<ULoginFirstWidget>(GetWorld(),loginWidget);
		if(loginUI != nullptr)
		{
			loginUI->AddToViewport();
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
			
			// 스팀서버 접속이라면
			
			if(IOnlineSubsystem::Get()->GetSubsystemName()=="STEAM")
			{
				if (SteamAPI_Init())
				{
					CSteamID steamID = SteamUser()->GetSteamID();
					const char* nickname = SteamFriends()->GetPersonaName();
					UE_LOG(LogTemp, Log, TEXT("Steam Nickname: %s"), UTF8_TO_TCHAR(nickname));
					//loginUI->eText_hostName->SetHintText(FText::FromString(UTF8_TO_TCHAR(nickname)));
					loginUI->eText_hostName->SetText(FText::FromString(UTF8_TO_TCHAR(nickname)));
				}
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,"No Steam login");
				
				loginUI->eText_hostName->SetHintText(FText::FromString(TEXT("닉네임을 입력해주세요")));
			}
		}
	}
	
	
}
