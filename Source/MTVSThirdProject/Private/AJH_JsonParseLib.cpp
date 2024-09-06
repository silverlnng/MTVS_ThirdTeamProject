// Fill out your copyright notice in the Description page of Project Settings.


#include "AJH_JsonParseLib.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonWriter.h"

FString UAJH_JsonParseLib::WeatherJsonParse(const FString& json)
{
	// �����⸦ �����
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	// �Ľ� ����� ���� ���� ����
	TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());
	// �ؼ��� �Ѵ�.
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
	// ��ȯ�Ѵ�
	return returnValue;
}

FString UAJH_JsonParseLib::MakeWeatherJson(const TMap<FString, FString> source)
{
	// source�� JsonObject �������� �����
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());

	for (TPair<FString, FString> pair : source)
	{
		jsonObject->SetStringField(pair.Key, pair.Value);
	}

	// writer �� ���� JsonObject �� ���ڵ��ؼ�
	FString json;
	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create(&json);
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);
	// ��ȯ�Ѵ�.
	return json;
}
