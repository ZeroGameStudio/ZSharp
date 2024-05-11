// Copyright Zero Games. All Rights Reserved.


#include "ZType.h"

#include "Interop/ZType_Interop.h"

const FString& ZSharp::FZType::GetName() const
{
	if (!CachedName)
	{
		FString data;
		FZType_Interop::GGetName(Handle, data);
		CachedName = MoveTemp(data);
	}

	return *CachedName;
}

bool ZSharp::FZType::IsZCallable() const
{
	return true;
}

ZSharp::FZConjugateHandle ZSharp::FZType::BuildConjugate(void* unmanaged) const
{
	if (!IsZCallable())
	{
		return FZConjugateHandle{};
	}
	
	return FZType_Interop::GBuildConjugate(Handle, unmanaged);
}


