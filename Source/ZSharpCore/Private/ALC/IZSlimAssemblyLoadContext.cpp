// Copyright Zero Games. All Rights Reserved.


#include "ALC/IZSlimAssemblyLoadContext.h"

ZSharp::EZLoadAssemblyErrorCode ZSharp::IZSlimAssemblyLoadContext::LoadAssembly(const FString& path, void* args)
{
	TArray<uint8> content;
	return FFileHelper::LoadFileToArray(content, *path, FILEREAD_Silent) ? LoadAssembly(content, args) : EZLoadAssemblyErrorCode::UnknownError;
}


