// Copyright Zero Games. All Rights Reserved.


#include "ZMethodInfo.h"

#include "ZCall/ZCallToManagedScope.h"
#include "Interop/ZMethodInfo_Interop.h"
#include "ZCall/ZCallBuffer.h"

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

int32 ZSharp::FZMethodInfo::GetNumSlots() const
{
	if (!CachedNumSlots)
	{
		CachedNumSlots = FZMethodInfo_Interop::GGetNumSlots(Handle);
	}
	
	return *CachedNumSlots;
}

ZCALL_TO_MANAGED_AUTO_BUFFER(ZSharp::FZMethodInfo::Invoke)
{
	if (!IsZCallable())
	{
		return -1;
	}

	{
		FZCallToManagedScope _;

		const int32 numSlots = GetNumSlots();
		if (numSlots > 0)
		{
			FZCallBufferSlot* slots = StaticCast<FZCallBufferSlot*>(UE_VSTACK_ALLOC(,numSlots * sizeof(FZCallBufferSlot)));
			FZCallBuffer buffer { slots };
			initialize(buffer, 0);
	
			return FZMethodInfo_Interop::GInvoke(Handle, &buffer);
		}
		
		return FZMethodInfo_Interop::GInvoke(Handle, nullptr);
	}
}

ZCALL_TO_MANAGED_USER_BUFFER(ZSharp::FZMethodInfo::Invoke)
{
	if (!IsZCallable())
	{
		return -1;
	}

	{
		FZCallToManagedScope _;

		return FZMethodInfo_Interop::GInvoke(Handle, buffer());
	}
}

ZCALL_TO_MANAGED_USER_BUFFER_CHECKED(ZSharp::FZMethodInfo::Invoke)
{
	if (!IsZCallable())
	{
		return -1;
	}

	if (numSlots != GetNumSlots())
	{
		return -2;
	}

	{
		FZCallToManagedScope _;

		return FZMethodInfo_Interop::GInvoke(Handle, buffer());
	}
}


