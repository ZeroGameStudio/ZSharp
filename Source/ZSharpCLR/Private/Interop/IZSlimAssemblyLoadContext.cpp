// Copyright Zero Games. All Rights Reserved.


#include "Interop/IZSlimAssemblyLoadContext.h"

int32 ZSharp::IZSlimAssemblyLoadContext::LoadAssembly(const FString& path, void* args)
{
	TArray<uint8> content;
	return FFileHelper::LoadFileToArray(content, *path, FILEREAD_Silent) ? LoadAssembly(content, args) : -1;
}
