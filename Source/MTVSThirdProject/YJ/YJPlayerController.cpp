// Fill out your copyright notice in the Description page of Project Settings.


#include "YJPlayerController.h"

void AYJPlayerController::ServerLocationChange_Implementation(const FVector& loc)
{
	GetPawn()->SetActorLocation(loc);
}
