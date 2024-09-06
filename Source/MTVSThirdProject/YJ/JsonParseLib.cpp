// Fill out your copyright notice in the Description page of Project Settings.


#include "JsonParseLib.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"

FString UJsonParseLib::JsonParse(const FString& json)
{
	// 여기서 json은  String
	
	//리더기를 만들고
	TSharedRef<TJsonReader<TCHAR>> Reader =TJsonReaderFactory<TCHAR>::Create(json);

	// 파싱결과를 담을 변수 선언
	// MakeShareable : 스마트 포인터
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());

	FString returnValue;
	//해석
	// 리더기 , json Object
	if(FJsonSerializer::Deserialize(Reader,result))
	{
		//result->GetObjectField(TEXT("today"))
		TSharedPtr<FJsonObject> todayObj = result->GetObjectField(TEXT("today"));
		FString temperatureStr = todayObj->GetStringField(TEXT("temperature"));
		FString weatherStr = todayObj->GetStringField(TEXT("weather"));
		//TSharedPtr<FJsonValue> temperature = todayObj->GetField(TEXT("temperature"),EJson::String);
		returnValue.Append(FString::Printf(TEXT("Today :temperature : %s\n"), *temperatureStr));
		returnValue.Append(FString::Printf(TEXT("Today :weather : %s\n"), *weatherStr));
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
