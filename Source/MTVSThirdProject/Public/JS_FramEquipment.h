// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "JS_LandTileActor.h"
#include "JS_FramEquipment.generated.h"

UCLASS()
class MTVSTHIRDPROJECT_API AJS_FramEquipment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJS_FramEquipment();

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
	class UStaticMeshComponent* framEquipmentComp;


	AJS_LandTileActor* tileActor;

	//농기구 아이템 번호
	int32 hoe = 95001;
	int32 pick = 95002;
	int32 sickle = 65003;
	int32 axe = 95004;
	int32 pail = 95005;
	int32 fishing_Rod = 95006;

	//물 채우기
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Pail")
	int32 haveWater = 0;

	//낚시
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Fishing")
	float currentTime = 0;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Fishing")
	int32 fishingCount = 0;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Fishing")
	int32 jewelCount = 0;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Fishing")
	int32 fishCount = 0;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Fishing")
	int32 trashCount = 0;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void StartFishing();
	void GetHaveWater();

	UFUNCTION(Server, Reliable)
	void Server_OverlapBegin(AActor* OtherActor);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_OverlapBegin(AActor* OtherActor);

	UFUNCTION(Server, Reliable)
	void Server_StartFishing();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartFishing();

	UFUNCTION(Server, Reliable)
	void Server_GetHaveWater();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_GetHaveWater();



};
