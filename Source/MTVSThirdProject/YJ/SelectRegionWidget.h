// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectRegionWidget.generated.h"

/**
 * 
 */
UCLASS()
class MTVSTHIRDPROJECT_API USelectRegionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ComboBox_Region;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Move;

	UFUNCTION()
	void OnClickedBtnMove();

	UPROPERTY(EditAnywhere)
	TMap<FString,FVector> regionMap;
};
