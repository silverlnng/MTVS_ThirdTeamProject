// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NPCCharacter.generated.h"

UCLASS()
class MTVSTHIRDPROJECT_API ANPCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPCCharacter();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;
	
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(EditDefaultsOnly)
	class AYJHUD* MyHUD;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	class UNPCWidget* NPC_UI;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category=Quest)
	int32 QuestNum =1; // npc 인스턴스 마다 다르게 설정할것

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	TArray<FString> TotalCSVLines;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	TArray<FString> EachCSVLines;
	
	UFUNCTION()
	void ReadCSV();
	
	UFUNCTION()
	void MakeEachCSVLines(int32 num);

	UFUNCTION()
	void ReadEachLinesNum(int32 num);
};
