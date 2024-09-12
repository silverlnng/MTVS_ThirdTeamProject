// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGameMode.h"
#include "LoginFirstWidget.h"
#include "OnlineSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableText.h"
#include "GameFramework/PlayerState.h"

void ALoginGameMode::BeginPlay()
{
	Super::BeginPlay();
	if(loginWidget != nullptr)
	{
		loginUI = CreateWidget<ULoginFirstWidget>(GetWorld(),loginWidget);
		if(loginUI != nullptr)
		{
			loginUI->AddToViewport();
			GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
			
			// 스팀서버 접속이라면

			FTimerHandle handle;
			GetWorldTimerManager().SetTimer(handle,[this]()
			{
				if(IOnlineSubsystem::Get()->GetSubsystemName()=="STEAM")
				{
					const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
					FString name = *LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId()->ToString();
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,*name);
					//loginUI->eText_hostName->SetText(FText::FromString(name));
					loginUI->eText_hostName->SetHintText(FText::FromString(name));
						//
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,"No such login");
					const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
					FString name = *LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId()->ToString();
					
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,*name);
					//loginUI->eText_hostName->SetText(FText::FromString(name));
					loginUI->eText_hostName->SetHintText(FText::FromString(name));
				}
			},3.f,false);
			
			
			
		}
	}
	
	
}
