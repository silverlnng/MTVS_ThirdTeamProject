// Fill out your copyright notice in the Description page of Project Settings.


#include "AJH_JsonParseLib.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"

FString UAJH_JsonParseLib::WeatherJsonParse(const FString& json)
{
	// 리더기를 만들고
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	// 파싱 결과를 담을 변수 선언
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());
	// 해석을 한다.
	FString returnValue;
	if (FJsonSerializer::Deserialize(reader, result))
	{
		TArray<TSharedPtr<FJsonValue>> paraseDataList = result->GetArrayField(TEXT("items"));
		for (TSharedPtr<FJsonValue> data : paraseDataList)
		{
			FString today = data->AsObject()->GetStringField(TEXT("Today_nm"));
			returnValue.Append(FString::Printf(TEXT("Today : %s"), *today));
		}
	}
	// 반환한다
	return returnValue;
}

FString UAJH_JsonParseLib::MakeWeatherJson(const TMap<FString, FString> source)
{
	// source를 JsonObject 형식으로 만든다
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());

	for (TPair<FString, FString> pair : source)
	{
		jsonObject->SetStringField(pair.Key, pair.Value);
	}

	// writer 를 만들어서 JsonObject 를 인코딩해서
	FString json;
	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create(&json);
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);
	// 반환한다.
	return json;
}
