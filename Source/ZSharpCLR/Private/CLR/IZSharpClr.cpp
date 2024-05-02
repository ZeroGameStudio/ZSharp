// Copyright Zero Games. All Rights Reserved.

#include "CLR/IZSharpClr.h"

#include "ZSharpCLR.h"
#include "ZGenericClr.h"

namespace ZSharp::IZSharpClr_Private
{
	static TAtomic<uint64> GAnonymousSlimAlcNameSerial = 0;
}

ZSharp::IZSharpClr& ZSharp::IZSharpClr::Get()
{
	static FZSharpClr GSingleton;

	return GSingleton;
}

int32 ZSharp::IZSharpClr::Run(const FString& path, void* args, const FString& alcName)
{
	FString actualAlcName = alcName.IsEmpty() ? FString::Printf(TEXT("__Anonymous%llu"), IZSharpClr_Private::GAnonymousSlimAlcNameSerial++) : alcName;
	IZSlimAssemblyLoadContext* alc = CreateSlimAlc(actualAlcName);
	if (!alc)
	{
		return -1;
	}

	ON_SCOPE_EXIT{ alc->Unload(); };

	return alc->LoadAssembly(path, args);
}

int32 ZSharp::IZSharpClr::RunAsync(const FString& path, void* args, const FString& alcName)
{
	FString actualAlcName = alcName.IsEmpty() ? FString::Printf(TEXT("__Anonymous%llu"), IZSharpClr_Private::GAnonymousSlimAlcNameSerial++) : alcName;
	IZSlimAssemblyLoadContext* alc = CreateSlimAlc(actualAlcName);
	if (!alc)
	{
		return -1;
	}

	return alc->LoadAssembly(path, args);
}


