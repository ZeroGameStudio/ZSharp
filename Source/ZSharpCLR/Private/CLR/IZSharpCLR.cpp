// Copyright Zero Games. All Rights Reserved.

#include "CLR/IZSharpCLR.h"

#include "ZSharpCLR.h"
#include "ZGenericCLR.h"

namespace ZSharp::IZSharpCLR_Private
{
	static TAtomic<uint64> GAnonymousSlimALCNameSerial = 0;
}

ZSharp::IZSharpCLR& ZSharp::IZSharpCLR::Get()
{
	static FZSharpCLR GSingleton;

	return GSingleton;
}

int32 ZSharp::IZSharpCLR::Run(const FString& path, void* args)
{
	IZSlimAssemblyLoadContext* alc = CreateSlimALC(FString::Printf(TEXT("__Anonymous%llu"), IZSharpCLR_Private::GAnonymousSlimALCNameSerial++));
	if (!alc)
	{
		return -1;
	}

	ON_SCOPE_EXIT{ UnloadSlimALC(alc); };

	return alc->LoadAssembly(path, args);
}

int32 ZSharp::IZSharpCLR::RunAsync(const FString& path, void* args)
{
	IZSlimAssemblyLoadContext* alc = CreateSlimALC(FString::Printf(TEXT("__Anonymous%llu"), IZSharpCLR_Private::GAnonymousSlimALCNameSerial++));
	if (!alc)
	{
		return -1;
	}

	return alc->LoadAssembly(path, args);
}


