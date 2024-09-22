// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "YJPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MTVSTHIRDPROJECT_API AYJPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere)
	class ANPCCharacter* CurNPC;
	
};
