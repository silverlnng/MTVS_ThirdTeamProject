// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CSVManager.generated.h"

/**
 * 
 */
UCLASS()
class MTVSTHIRDPROJECT_API UCSVManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable,category="CSV",meta=(Keywords="Save"))
	static bool SaveArrayText(FString SaveDirectory,FString fileName,TArray<FString> SaveText,bool AllowOverWriting);
	
};
