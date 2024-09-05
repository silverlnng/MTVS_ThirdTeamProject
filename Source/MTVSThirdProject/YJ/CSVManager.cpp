// Fill out your copyright notice in the Description page of Project Settings.


#include "CSVManager.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFileManager.h"

bool UCSVManager::SaveArrayText(FString SaveDirectory, FString fileName, TArray<FString> SaveText,
	bool AllowOverWriting)
{
	SaveDirectory += "\\";
	SaveDirectory += fileName;
	
	if (!AllowOverWriting)
	{
		// AllowOverWriting : True가 덮어쓰기 가능 (수정가능) 설정값.
		// SaveDirectory: 해당디렉토리에 파일이 존재하면 true , 아니라면 false
		
		if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory))
		{
			return false;
		}
	}
	FString FinalString = "";
	
	for (FString& Each : SaveText)
	{
		FinalString+=Each;
		FinalString+=LINE_TERMINATOR;
	}
	// 1열에만 저장 됨 ==> 이미들어있는 행값을 찾아서 값 넣는 방법 ?
	return FFileHelper::SaveStringToFile(FinalString,*SaveDirectory);
}
