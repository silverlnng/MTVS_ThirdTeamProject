// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_DefaultPlant.h"
#include "JS_FramEquipment.h"
#include "JS_ObstacleActor.generated.h"

UCLASS()
class MTVSTHIRDPROJECT_API AJS_ObstacleActor : public AActor, public IJS_DefaultPlant
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_ObstacleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* staticMeshComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actors")
	TSubclassOf<class AJS_ObstacleActor> TreeFactory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actors")
	TSubclassOf<class AJS_ObstacleActor> GressFactory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actors")
	TSubclassOf<class AJS_ObstacleActor> RockFactory;

	// Sound
	// tree------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* treeDistroySound1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* treeDistroySound2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* treeDistroySound3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* treeDistroySound4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* treeGetDamageSound;

	// tree------------------------------------------------

	// rock------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* rockDistroySound1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* rockDistroySound2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* rockDistroySound3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* rockDistroySound4;
	// rock------------------------------------------------

	// gress------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* gressDistroySound1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* gressDistroySound2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* gressDistroySound3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* gressDistroySound4;
	// gress------------------------------------------------
	
	//농기구 태그 확인용
	AJS_FramEquipment* framEquipment;

	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Set")
	bool btree = false;
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Set")
	int32 treeMaxHP = 5;
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Set")
	int32 treeCurHP = treeMaxHP;
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Set")
	int32 maxHP = 1;
	UPROPERTY(Replicated, BlueprintReadWrite, Category = "Set")
	int32 curHP = maxHP;

	virtual void GetDamage_Implementation(bool damage) override;
	virtual void SetCurHP_Implementation(float amount) override;
	virtual void Death_Implementation() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Server, Reliable)
	void Server_GetDamage(bool damage);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_GetDamage(bool damage);

	UFUNCTION(Server, Reliable)
	void Server_SetCurHP(float amount);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetCurHP(float amount);

	UFUNCTION(Server, Reliable)
	void Server_Death();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Death();

	//Sound
	void PlayObstacleSound();

	UFUNCTION(Server, Reliable)
	void Server_PlayObstacleSound();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayObstacleSound();

	UFUNCTION()
	void SetSoundfile();

};
