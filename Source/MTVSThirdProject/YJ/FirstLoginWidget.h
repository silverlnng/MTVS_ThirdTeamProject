// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FirstLoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class MTVSTHIRDPROJECT_API UFirstLoginWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "MySettings")
    class UEditableText* eText_hostName;
	
};
