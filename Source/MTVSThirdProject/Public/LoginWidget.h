// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class MTVSTHIRDPROJECT_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;
public:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget), Category="MySettings")
	class UEditableText* eText_roomName;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "MySettings")
	class UEditableText* eText_hostName;

	
	
	UPROPERTY(VisibleAnywhere,meta=(BindWidget),Category="MySettings")
	class UButton* btn_create;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "MySettings")
	class UButton* btn_findSessions;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "MySettings")
	class UScrollBox* sb_roomList;

	UPROPERTY(BlueprintReadOnly)
	class UNetWorkGameInstance* gi;
	//게임프레임워크 시스템은 줄여서 변수명 자주씀!

	UFUNCTION()
	void OnClickedCreateButton();

	UPROPERTY(EditAnywhere, Category="MySettings")
	TSubclassOf<class USessionSlotWidget> slotWidget;
	
	UFUNCTION()
	void OnSlotCreated(FString roomName, FString hostName, int32 currentPlayers, int32 maxPlayers, int32 ping, int32 sessionIdx);

	UFUNCTION()
	void OnClickedFindSessionsButton();
};
