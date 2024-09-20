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

void UCSVManager::ReadCSV()
{
	FString CSVPath = FPaths::ProjectDir() / TEXT("NPCData.csv");

	// 파일 내용을 저장할 변수
	FString FileContent;
	
	// 파일 읽기
	if (FFileHelper::LoadFileToString(FileContent, *CSVPath))
	{
		// 한 줄씩 나누기
		TArray<FString> Lines;
		FileContent.ParseIntoArrayLines(Lines);

		for (const FString& Line : Lines)
		{
			// 쉼표로 구분된 각 항목 분리
			TArray<FString> Columns;
			Line.ParseIntoArray(Columns, TEXT(","), true);

			// 각 열 데이터 처리 (예: Name과 Score가 있다고 가정)
			if (Columns.Num() >= 2)
			{
				FString Name = Columns[0]; // 첫 번째 열: 이름
				int32 Score = FCString::Atoi(*Columns[1]); // 두 번째 열: 점수

				// 여기서 UI에 표시할 수 있음
				//AddRowToUI(Name, Score);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load CSV file: %s"), *CSVPath);
	}
}
