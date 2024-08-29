// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "NetWorkGameInstance.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FSearchSessionDele,FString,roomName,FString,hostName,int32,curPlayer,
int32,maxPlayer,int32, pingSpeed,int32,sessionIdx);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FServerResponseDele);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFindButtonStatus,bool, bisEnabled);

UCLASS()
class MTVSTHIRDPROJECT_API UNetWorkGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	IOnlineSessionPtr sessionInterface;
	//세션에 관련된 모든 컨트롤을 하는 기본클래스

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category="MySettings")
	FSearchSessionDele onCreateSlot;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "MySettings")
	FServerResponseDele onNewSearchComplete;

	UPROPERTY(EditAnywhere, BlueprintAssignable, Category = "MySettings")
	FFindButtonStatus onFindButtonToggle;

	
	void CreateMySession(FString roomName,FString hostName, int32 playerCount);		// 요청한 결과를 받기 위한 함수
	void FindMySession();
	void JoinMySession(int32 roomNumber);
	void ExitMySession();
	void SetSessionName(FString name);
	FORCEINLINE FString GetSessionName(){return mySessionName.ToString();}

private:
	FName mySessionName = FName("first Session");
	TSharedPtr<class FOnlineSessionSearch> sessionSearch;	//OnlineSessionSearch 는 
	
	void OnCreatedSession(FName sessionName, bool bWasSuccessful);	//서버에 세션생성을 요청하기 위한 함수
	void OnFoundSession(bool bwasSuccessful);
	void OnJoinedSession(FName SesssionName, EOnJoinSessionCompleteResult::Type result);
	void OnDestroyedSesssion(FName sessionName, bool bwasSuccessful);	
	
};


