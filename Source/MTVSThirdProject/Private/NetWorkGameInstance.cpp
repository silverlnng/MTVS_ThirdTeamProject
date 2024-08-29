// Fill out your copyright notice in the Description page of Project Settings.


#include "NetWorkGameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"

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
	//SessionSettings.bUsesPresence =true;
	SessionSettings.bShouldAdvertise = true; //다른사람이 세션검색할경우 노출되도록 ( 검색이 가능하도록 )
	SessionSettings.bUseLobbiesIfAvailable=true;  //로비의 사용여부
	SessionSettings.NumPublicConnections=playerCount;
	//SessionSettings.NumPrivateConnections //호스트가 초대를 해야만 입장가능

	//커스텀 설정값을 추가하기
	SessionSettings.Set(FName("Room Name"),roomName,EOnlineDataAdvertisementType::Type::ViaOnlineServiceAndPing);
	SessionSettings.Set(FName("Host Name"),hostName,EOnlineDataAdvertisementType::Type::ViaOnlineServiceAndPing);
	
	sessionInterface->CreateSession(0, mySessionName,SessionSettings);
	//서버에  이런 세팅값으로 만들어달라는 요청 ( 호출시점 에 session이 완성된게 아님 )
	UE_LOG(LogTemp,Warning,TEXT("Try to create Session"));
	UE_LOG(LogTemp,Warning,TEXT("current platform : %s"),*IOnlineSubsystem::Get()->GetSubsystemName().ToString());
}

void UNetWorkGameInstance::OnCreatedSession(FName sessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp,Warning,TEXT("Session Name: %s") , *sessionName.ToString());
	UE_LOG(LogTemp,Warning,TEXT("Session create: %s") , bWasSuccessful ? *FString("Success!") : *FString("Fail"));

	//멀티플레이를 할 맵으로 이동한다. 맵의 경로 작성해주기
	///Script/Engine.World'/Game/Maps/BattleMap.BattleMap' 에서 상대 경로 만 넣어주면 됨
	GetWorld()->ServerTravel("/Game/YJ/MainGameMap?Listen",true);
	//지금현재 리슨서버이기때문에  ?listen 으로 설정 
}

void UNetWorkGameInstance::FindMySession()
{
	// 세션 검색 조건을 설정하기
	sessionSearch = MakeShareable(new FOnlineSessionSearch());
	check(sessionSearch);
	sessionSearch->bIsLanQuery = true;
	sessionSearch->MaxSearchResults = 10;
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Type::Equals);


	// 서버에 세션 검색을 요청하기
	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
	
}

void UNetWorkGameInstance::OnFoundSession(bool bwasSuccessful)
{
	check(sessionSearch);
	
	TArray<FOnlineSessionSearchResult> results = sessionSearch->SearchResults;

	UE_LOG(LogTemp, Warning, TEXT("Find Results: %s"), bwasSuccessful ? *FString("Success!") : *FString("Failed..."));

	if (bwasSuccessful)
	{
		int32 sessionNum = results.Num();
		UE_LOG(LogTemp, Warning, TEXT("Session Count: %d"), results.Num());

		onNewSearchComplete.Broadcast();

		//for (const FOnlineSessionSearchResult& result : results)
		for (int32 i = 0; i < results.Num(); i++)
		{
			FString foundRoomName;
			results[i].Session.SessionSettings.Get(FName("Room Name"), foundRoomName);
			FString foundHostName;
			results[i].Session.SessionSettings.Get(FName("Host Name"), foundHostName);

			int32 maxPlayerCount = results[i].Session.SessionSettings.NumPublicConnections;
			int32 currentPlayerCount = maxPlayerCount - results[i].Session.NumOpenPublicConnections;

			int32 pingSpeed = results[i].PingInMs;

			// 로그로 확인하기
			UE_LOG(LogTemp, Warning, TEXT("Room Name: %s\nHost Name: %s\nPlayer Count: (%d/%d)\nPing: %d ms\n\n"), *foundRoomName, *foundHostName, currentPlayerCount, maxPlayerCount, pingSpeed);

			// 델리게이트 이벤트 실행하기
			onCreateSlot.Broadcast(foundRoomName, foundHostName, currentPlayerCount, maxPlayerCount, pingSpeed, i);
		}

		onFindButtonToggle.Broadcast(true);
	}
}

void UNetWorkGameInstance::JoinMySession(int32 roomNumber)
{
	sessionInterface->JoinSession(0, mySessionName, sessionSearch->SearchResults[roomNumber]);
}

void UNetWorkGameInstance::ExitMySession()
{
	sessionInterface->DestroySession(mySessionName);
}

void UNetWorkGameInstance::SetSessionName(FString name)
{
	mySessionName = FName(*name);
}

void UNetWorkGameInstance::OnJoinedSession(FName SesssionName, EOnJoinSessionCompleteResult::Type result)
{
	UE_LOG(LogTemp, Warning, TEXT("Joined Session: %s"), *SesssionName.ToString());

	switch (result)
	{
	case EOnJoinSessionCompleteResult::Success:
		{
			UE_LOG(LogTemp, Warning, TEXT("Join Success!"));

			APlayerController* pc = GetWorld()->GetFirstPlayerController();
			FString url;
			sessionInterface->GetResolvedConnectString(SesssionName, url, NAME_GamePort);
			UE_LOG(LogTemp, Warning, TEXT("url: %s"), *url);

			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);

			break;
		}
	case EOnJoinSessionCompleteResult::SessionIsFull:
		UE_LOG(LogTemp, Warning, TEXT("Session is full..."));
		break;
	case EOnJoinSessionCompleteResult::SessionDoesNotExist:
		UE_LOG(LogTemp, Warning, TEXT("Session Does Not Exist..."));
		break;
	case EOnJoinSessionCompleteResult::CouldNotRetrieveAddress:
		UE_LOG(LogTemp, Warning, TEXT("Could Not Retrieve Address..."));
		break;
	case EOnJoinSessionCompleteResult::AlreadyInSession:
		UE_LOG(LogTemp, Warning, TEXT("You are already in this session..."));
		break;
	case EOnJoinSessionCompleteResult::UnknownError:
		UE_LOG(LogTemp, Warning, TEXT("Unknown Error occurred!"));
		break;
	default:
		break;
	}	
}

void UNetWorkGameInstance::OnDestroyedSesssion(FName sessionName, bool bwasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Destory Session: %s"), bwasSuccessful ? *FString("Success!") : *FString("Failed..."));

	if (bwasSuccessful)
	{
		APlayerController* pc = GetWorld()->GetFirstPlayerController();

		if (pc != nullptr)
		{
			pc->ClientTravel(FString("/Game/Maps/LobbyLevel"), ETravelType::TRAVEL_Absolute);
			// 
		}
	}
}
