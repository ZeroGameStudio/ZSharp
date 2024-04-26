// Copyright Zero Games. All Rights Reserved.


#include "ZCall/ZStaticExportZCall.h"

#include "ZCallResolver_Export.h"

ZSharp::FZStaticExportZCall::FZStaticExportZCall(const FString& name, const TFunction<int32(FZCallBuffer*)>& function)
{
	FZCallResolver_Export::RegisterFunction(name, function);
}


