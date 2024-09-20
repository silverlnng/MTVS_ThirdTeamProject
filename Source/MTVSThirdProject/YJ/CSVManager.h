// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CSVManager.generated.h"

/**
 * 
 */
UCLASS()
class MTVSTHIRDPROJECT_API ACSVManager : public AActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable,category="CSV",meta=(Keywords="Save"))
	static bool SaveArrayText(FString SaveDirectory,FString fileName,TArray<FString> SaveText,bool AllowOverWriting);

	UFUNCTION(BlueprintCallable,category="CSV",meta=(Keywords="Save"))
	void ReadCSV();

	UPROPERTY(EditDefaultsOnly)
	class AYJHUD* MyHUD;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UNPCWidget* NPC_UI;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	TArray<FString> CSVLines;

	UFUNCTION()
	void CSVLinesNum(int32 num);
};
