// Fill out your copyright notice in the Description page of Project Settings.


#include "NetWorkGameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"

void UNetWorkGameInstance::Init()
{
	Super::Init();
	sessionInterface = IOnlineSubsystem::Get()->GetSessionInterface();
	
	if(sessionInterface != nullptr)
	{
		//서버쪽에서 Delegate으로 이벤트 값을 받을 함수를 연결
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetWorkGameInstance::OnCreatedSession);
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this,&UNetWorkGameInstance::OnFoundSession);
		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this,&UNetWorkGameInstance::OnJoinedSession);
		sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this,&UNetWorkGameInstance::OnDestroyedSesssion);
		//oncreate , create
		//서버에 요청하고 응답을 받는데 지연시간이 있음
		//create하고 다만들어지면  create가 완료 되었을때 실행시킬 함수 OnCreatedSession 를 만들기
		//OnJoinSessionCompleteDelegates : 매개변수 필요 F12 눌러서 확인하기  
	}
}

void UNetWorkGameInstance::CreateMySession(FString roomName, FString hostName, int32 playerCount)
{
	FOnlineSessionSettings SessionSettings;	//SessionSettings구조체에 하나씩 설정값을 세팅
	
	SessionSettings.bIsDedicated = false; // 데디케이트 서버 사용안함
	SessionSettings.bAllowInvites = true; //초대기능 사용
	SessionSettings.bAllowJoinInProgress=true; // 진행중에도 들어오는것을 사용
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName()=="NULL"? true:false;
	// 접속하는 방법이 랜 경유 , 스팀서버 경유 두가지 있는데 랜 경유이면 null 문자열 반환, 스팀이면 steam 문자열 반환
	
	SessionSettings.bShouldAdvertise = true; //다른사람이 세션검색할경우 노출되도록 ( 검색이 가능하도록 )
	SessionSettings.bUseLobbiesIfAvailable=true;  //로비의 사용여부
	SessionSettings.NumPublicConnections=playerCount;
	//SessionSettings.NumPrivateConnections //호스트가 초대를 해야만 입장가능

	//커스텀 설정값을 추가하기
	SessionSettings.Set(FName("Room Name"),roomName,EOnlineDataAdvertisementType::Type::ViaOnlineServiceAndPing);
	SessionSettings.Set(FName("Host Name"),hostName,EOnlineDataAdvertisementType::Type::ViaOnlineServiceAndPing);
	
	sessionInterface->CreateSession(0,mySessionName,SessionSettings);
	//서버에  이런 세팅값으로 만들어달라는 요청 ( 호출시점 에 session이 완성된게 아님 )
	UE_LOG(LogTemp,Warning,TEXT("Try to create Session"));
	UE_LOG(LogTemp,Warning,TEXT("current platform : %s"),*IOnlineSubsystem::Get()->GetSubsystemName().ToString());
}

void UNetWorkGameInstance::FindMySession()
{
	
}

void UNetWorkGameInstance::JoinMySession(int32 roomNumber)
{
}

void UNetWorkGameInstance::ExitMySession()
{
}

void UNetWorkGameInstance::SetSessionName(FString name)
{
}

void UNetWorkGameInstance::OnCreatedSession(FName sessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp,Warning,TEXT("Session Name: %s") , *sessionName.ToString());
	UE_LOG(LogTemp,Warning,TEXT("Session create: %s") , bWasSuccessful ? *FString("Success!") : *FString("Fail"));

	//멀티플레이를 할 맵으로 이동한다. 맵의 경로 작성해주기
	///Script/Engine.World'/Game/Maps/BattleMap.BattleMap' 에서 상대 경로 만 넣어주면 됨
	GetWorld()->ServerTravel("/Game/YJ/MainGameMap?Listen",true);

	// /Script/Engine.World'/Game/YJ/LobbyMap.LobbyMap'
	// /Script/Engine.World'/Game/YJ/MainGameMap.MainGameMap'
	
	//지금현재 리슨서버이기때문에  ?listen 으로 설정 
}

void UNetWorkGameInstance::OnFoundSession(bool bwasSuccessful)
{
}

void UNetWorkGameInstance::OnJoinedSession(FName SesssionName, EOnJoinSessionCompleteResult::Type result)
{
}

void UNetWorkGameInstance::OnDestroyedSesssion(FName sessionName, bool bwasSuccessful)
{
}
