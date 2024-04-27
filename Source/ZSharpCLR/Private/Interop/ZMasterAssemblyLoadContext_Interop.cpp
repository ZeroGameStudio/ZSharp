// Copyright Zero Games. All Rights Reserved.


#include "ZMasterAssemblyLoadContext_Interop.h"

#include "CLR/IZSharpCLR.h"
#include "Interop/IZMasterAssemblyLoadContext.h"

int32 ZSharp::FZMasterAssemblyLoadContext_Interop::ZCallByHandle(FZCallHandle handle, FZCallBuffer* buffer)
{
	const IZMasterAssemblyLoadContext* alc = IZSharpCLR::Get().GetMasterALC();
	if (!alc)
	{
		return -1;
	}

	return alc->ZCall(handle, buffer);
}

int32 ZSharp::FZMasterAssemblyLoadContext_Interop::ZCallByName(const TCHAR* name, FZCallBuffer* buffer, FZCallHandle* outHandle)
{
	IZMasterAssemblyLoadContext* alc = IZSharpCLR::Get().GetMasterALC();
	if (!alc)
	{
		return -1;
	}

	return outHandle ? alc->ZCall(name, buffer, outHandle) : alc->ZCall(name, buffer);
}

void ZSharp::FZMasterAssemblyLoadContext_Interop::ZCallByHandle_AnyThread(FZCallHandle handle, FZCallBuffer* buffer, int32 numSlots)
{
	IZSharpCLR::Get().GetMasterALC_AnyThread([handle, buffer, numSlots](IZMasterAssemblyLoadContext* alc)
	{
		alc->ZCall_AnyThread(handle, buffer, numSlots);
	});
}

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext_Interop::GetZCallHandle(const TCHAR* name)
{
	IZMasterAssemblyLoadContext* alc = IZSharpCLR::Get().GetMasterALC();
	if (!alc)
	{
		return FZCallHandle{};
	}

	return alc->GetOrResolveZCallHandle({ name });
}


