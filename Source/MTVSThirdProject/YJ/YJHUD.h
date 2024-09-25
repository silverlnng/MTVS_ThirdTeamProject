// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "YJHUD.generated.h"

/**
 * 
 */

UCLASS()
class MTVSTHIRDPROJECT_API AYJHUD : public AHUD
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UNPCWidget> NPCWidgetFactory;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UNPCWidget* NPCUI;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UAJH_MainWidget> MainWidgetFactory;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UAJH_MainWidget* MainUI;

	
	// csv manager를 통해 text를 set하기
	
	
};
