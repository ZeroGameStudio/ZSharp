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

int32 ZSharp::FZPropertyInfo::Get(FZCallBuffer* buffer) const
{
	checkNoEntry();
	return -1;
}

int32 ZSharp::FZPropertyInfo::Set(FZCallBuffer* buffer) const
{
	checkNoEntry();
	return -1;
}


