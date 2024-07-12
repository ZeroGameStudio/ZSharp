// Copyright Zero Games. All Rights Reserved.


#include "IZExportedType.h"

FString ZSharp::IZExportedType::GetExportKey() const
{
	return FString::Printf(TEXT("%s.%s"), *GetModule(), *GetName());
}


