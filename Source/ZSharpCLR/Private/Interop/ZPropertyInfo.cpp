// Copyright Zero Games. All Rights Reserved.


#include "ZPropertyInfo.h"

const FString& ZSharp::FZPropertyInfo::GetName() const
{
	if (!CachedName)
	{
		FString data;
		data = "";
		CachedName = MoveTemp(data);
	}

	return *CachedName;
}

bool ZSharp::FZPropertyInfo::IsZCallable() const
{
	return true;
}


