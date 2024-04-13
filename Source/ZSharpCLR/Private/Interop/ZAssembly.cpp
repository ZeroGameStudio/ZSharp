// Copyright Zero Games. All Rights Reserved.


#include "ZAssembly.h"

#include "ZAssembly_Interop.h"

const FString& ZSharp::FZAssembly::GetName() const
{
	if (!CachedName)
	{
		FString data;
		FZAssembly_Interop::GGetName(Handle, data);
		CachedName = MoveTemp(data);
	}

	return *CachedName;
}

ZSharp::IZType* ZSharp::FZAssembly::GetType(const FString& name) const
{
	checkNoEntry();
	return nullptr;
}
