// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "JS_DefaultPlant.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UJS_DefaultPlant : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MTVSTHIRDPROJECT_API IJS_DefaultPlant
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	int32 curHP;
	int32 maxHP;
	float growTime;
	float checkDeltaTime = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Plant")
	void GetDamage(bool damage);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Plant")
	void SetCurHP(float amount);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Plant")
	void SpawnNextPlant(int32 index);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Plant")
	void Death();
};
