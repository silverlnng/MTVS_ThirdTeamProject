// Fill out your copyright notice in the Description page of Project Settings.


#include "YJ/JsonParseLib.h"

FString UJsonParseLib::JsonParse(const FString& json)
{
	//리더기를 만들고
	TSharedRef<TJsonReader<TCHAR>> Reader =TJsonReaderFactory<TCHAR>::Create(json);

	//파싱결과를 담을 변수 선언
	// MakeShareable : 스마트 포인터
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());

	FString returnValue;
	//해석
	// 리더기 , json Object
	if(FJsonSerializer::Deserialize(Reader,result))
	{
		TArray<TSharedPtr<FJsonValue>> parseDataList = result->GetArrayField(TEXT("items"));

		for(auto data : parseDataList)
		{
			// data 를 조절해서 원하는 대로 만들기
			//  "bk_nm": "노래하는 볼돼지",
			// "aut_nm": "김영진 글·그림",
			FString bookName =data->AsObject()->GetStringField("bk_nm");
			FString AuthName =data->AsObject()->GetStringField("aut_nm");
			returnValue.Append(FString::Printf(TEXT("BookName : %s / AuthName : %s\n"), *bookName,*AuthName));
			
		}
	}


	//변환
	return returnValue;
}

FString UJsonParseLib::MakeJson(const TMap<FString, FString> Source)
{
	// 소스를 jsonObject 형식으로 만든다
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	for(TPair<FString,FString> pair : Source)
	{
		JsonObject->SetStringField(pair.Key,pair.Value);
	}

	// writer 를 만들어서 JsonObject를 인코딩해서
	FString json;
	TSharedRef<TJsonWriter<TCHAR>> writer =TJsonWriterFactory<TCHAR>::Create(&json);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(),writer);

	// 반환 
	return json;
}
