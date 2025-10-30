// Copyright Zero Games. All Rights Reserved.

#include "ZConfig_Interop.h"

#include "Interop/ZInteropExceptionHelper.h"

namespace ZSharp::ZConfig_Interop_Private
{
	static void GetFileName(FConfigCacheIni* config, const TCHAR* baseIniName, FString& fileName)
	{
		fileName = config->GetConfigFilename(baseIniName);
	}

	static uint8 TryGetSection(FConfigCacheIni* config, const TCHAR* fileName, const TCHAR* section, TArray<FString>& values)
	{
		return config->GetSection(section, values, fileName);
	}

	static uint8 TryGetArray(FConfigCacheIni* config, const TCHAR* fileName, const TCHAR* section, const TCHAR* key, TArray<FString>& values)
	{
		return !!config->GetArray(section, key, values, fileName);
	}

	static uint8 TryGetString(FConfigCacheIni* config, const TCHAR* fileName, const TCHAR* section, const TCHAR* key, FString& value)
	{
		return config->GetString(section, key, value, fileName);
	}
}

void ZSharp::FZConfig_Interop::GetFileName(FConfigCacheIni* config, const TCHAR* baseIniName, FString& fileName)
{
	GUARD(ZConfig_Interop_Private::GetFileName(config, baseIniName, fileName));
}

uint8 ZSharp::FZConfig_Interop::TryGetSection(FConfigCacheIni* config, const TCHAR* fileName, const TCHAR* section, TArray<FString>& values)
{
	TRY
	{
		return ZConfig_Interop_Private::TryGetSection(config, fileName, section, values);
	}
	CATCHR(false)
}

uint8 ZSharp::FZConfig_Interop::TryGetArray(FConfigCacheIni* config, const TCHAR* fileName, const TCHAR* section, const TCHAR* key, TArray<FString>& values)
{
	TRY
	{
		return ZConfig_Interop_Private::TryGetArray(config, fileName, section, key, values);
	}
	CATCHR(false)
}

uint8 ZSharp::FZConfig_Interop::TryGetString(FConfigCacheIni* config, const TCHAR* fileName, const TCHAR* section, const TCHAR* key, FString& value)
{
	TRY
	{
		return ZConfig_Interop_Private::TryGetString(config, fileName, section, key, value);
	}
	CATCHR(false)
}


