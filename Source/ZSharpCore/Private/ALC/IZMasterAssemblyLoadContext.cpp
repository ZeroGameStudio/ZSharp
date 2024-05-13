// Copyright Zero Games. All Rights Reserved.


#include "ALC/IZMasterAssemblyLoadContext.h"

void ZSharp::IZMasterAssemblyLoadContext::LoadAssembly(const FString& path, void* args)
{
	TArray<uint8> content;
	if (FFileHelper::LoadFileToArray(content, *path, FILEREAD_Silent))
	{
		LoadAssembly(content, args);
	}
}


