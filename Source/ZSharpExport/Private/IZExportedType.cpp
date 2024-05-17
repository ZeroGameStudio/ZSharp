// Copyright Zero Games. All Rights Reserved.


#include "IZExportedType.h"

FString ZSharp::IZExportedType::GetInnerExportName() const
{
	return FString::Printf(TEXT("%s.%s"), *GetModule(), *GetName());
}


