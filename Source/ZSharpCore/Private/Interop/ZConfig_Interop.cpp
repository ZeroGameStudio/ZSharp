// Copyright Zero Games. All Rights Reserved.


#include "ZConfig_Interop.h"

void ZSharp::FZConfig_Interop::GetFileName(FConfigCacheIni* config, const TCHAR* baseIniName, FString& fileName)
{
	fileName = config->GetConfigFilename(baseIniName);
}

uint8 ZSharp::FZConfig_Interop::TryGetSection(FConfigCacheIni* config, const TCHAR* fileName, const TCHAR* section, FString& values)
{
	TArray<FString> valueArray;
	bool suc = config->GetSection(section, valueArray, fileName);
	values = FString::Join(valueArray, TEXT(";"));
	return suc;
}

uint8 ZSharp::FZConfig_Interop::TryGetArray(FConfigCacheIni* config, const TCHAR* fileName, const TCHAR* section, const TCHAR* key, FString& values)
{
	TArray<FString> valueArray;
	bool suc = !!config->GetArray(section, key, valueArray, fileName);
	values = FString::Join(valueArray, TEXT(";"));
	return suc;
}

uint8 ZSharp::FZConfig_Interop::TryGetString(FConfigCacheIni* config, const TCHAR* fileName, const TCHAR* section, const TCHAR* key, FString& value)
{
	return config->GetString(section, key, value, fileName);
}


