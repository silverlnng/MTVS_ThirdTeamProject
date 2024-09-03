// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AJH_JsonParseLib.generated.h"

/**
 * 
 */
UCLASS()
class MTVSTHIRDPROJECT_API UAJH_JsonParseLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static FString WeatherJsonParse(const FString& json);
	static FString MakeWeatherJson(const TMap<FString, FString> source);
	
};
