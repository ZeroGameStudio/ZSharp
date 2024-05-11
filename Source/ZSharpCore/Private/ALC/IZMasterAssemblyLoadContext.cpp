// Copyright Zero Games. All Rights Reserved.


#include "ALC/IZMasterAssemblyLoadContext.h"

const ZSharp::IZAssembly* ZSharp::IZMasterAssemblyLoadContext::LoadAssembly(const FString& path, void* args)
{
	TArray<uint8> content;
	return FFileHelper::LoadFileToArray(content, *path, FILEREAD_Silent) ? LoadAssembly(content, args) : nullptr;
}
