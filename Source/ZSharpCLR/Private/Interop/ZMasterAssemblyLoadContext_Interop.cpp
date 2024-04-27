// Copyright Zero Games. All Rights Reserved.


#include "ZMasterAssemblyLoadContext_Interop.h"

#include "CLR/IZSharpCLR.h"
#include "Interop/IZCallDispatcher.h"
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

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext_Interop::GetZCallHandle(const TCHAR* name)
{
	const IZMasterAssemblyLoadContext* alc = IZSharpCLR::Get().GetMasterALC();
	if (!alc)
	{
		return FZCallHandle{};
	}

	return alc->GetZCallHandle({ name });
}
