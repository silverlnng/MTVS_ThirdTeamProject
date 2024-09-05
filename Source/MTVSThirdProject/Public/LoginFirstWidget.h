// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginFirstWidget.generated.h"

/**
 * 
 */
UCLASS()
class MTVSTHIRDPROJECT_API ULoginFirstWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry & MyGeometry, float InDeltaTime) override;

public:
	
    UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "MySettings")
	class UEditableText* eText_hostName;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "MySettings")
	class UButton* btn_findSessions;

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "MySettings")
	class UComboBoxString* ComboBoxStr_Char;

	UPROPERTY(BlueprintReadOnly)
	class UNetWorkGameInstance* gi;

	UFUNCTION()
	void OnClickedFindSessionsButton();
	UFUNCTION()
	void SelectCharacter(FString SelectedItem, ESelectInfo::Type SelectionType);
};
