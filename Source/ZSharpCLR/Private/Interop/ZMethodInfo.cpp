// Copyright Zero Games. All Rights Reserved.


#include "ZMethodInfo.h"

#include "ZMethodInfo_Interop.h"

const FString& ZSharp::FZMethodInfo::GetName() const
{
	if (!CachedName)
	{
		FString data;
		FZMethodInfo_Interop::GGetName(Handle, data);
		CachedName = MoveTemp(data);
	}

	return *CachedName;
}

bool ZSharp::FZMethodInfo::IsZCallable() const
{
	return true;
}

int32 ZSharp::FZMethodInfo::Invoke(FZCallBuffer* buffer) const
{
	if (!IsZCallable())
	{
		return -1;
	}

	return FZMethodInfo_Interop::GInvoke(Handle, buffer);
}


