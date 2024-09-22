// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LoginGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MTVSTHIRDPROJECT_API ALoginGameMode : public AGameMode
{
	GENERATED_BODY()
public:	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere,Category="Mysettings")
	TSubclassOf<class UUserWidget> loginWidget;	

	UPROPERTY(EditDefaultsOnly, Category="Mysettings")
	class ULoginFirstWidget* loginUI;
};
