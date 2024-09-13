// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JsonParseLib.generated.h"

/**
 * 
 */
UCLASS()
class MTVSTHIRDPROJECT_API UJsonParseLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:	
	
	static FString JsonParse(const FString& json);

	static FString MakeJson(const TMap<FString, FString> Source);
	
	UFUNCTION(BlueprintCallable)
	static FString GetweatherString(const FString& json);
};
