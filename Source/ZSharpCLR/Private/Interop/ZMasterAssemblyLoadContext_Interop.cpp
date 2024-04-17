// Copyright Zero Games. All Rights Reserved.


#include "ZMasterAssemblyLoadContext_Interop.h"

#include "CLR/IZSharpCLR.h"
#include "Interop/IZCallDispatcher.h"
#include "Interop/IZMasterAssemblyLoadContext.h"


int32 ZSharp::FZMasterAssemblyLoadContext_Interop::ZCallByHandle(FZCallHandle handle, FZCallBuffer* buffer)
{
	IZMasterAssemblyLoadContext* alc = IZSharpCLR::Get().GetMasterALC();
	if (!alc)
	{
		return 0;
	}
	
	const IZCallDispatcher* dispatcher = alc->GetZCallDispatcher(handle);
	if (!dispatcher)
	{
		return 0;
	}

	dispatcher->Dispatch(buffer);
	
	return 1;
}

int32 ZSharp::FZMasterAssemblyLoadContext_Interop::ZCallByName(const TCHAR* name, FZCallBuffer* buffer, FZCallHandle* outHandle, uint8 bResolve)
{
	IZMasterAssemblyLoadContext* alc = IZSharpCLR::Get().GetMasterALC();
	if (!alc)
	{
		return 0;
	}
	
	const IZCallDispatcher* dispatcher = bResolve ? alc->GetOrResolveZCallDispatcher(name) : alc->GetZCallDispatcher(name);
	if (!dispatcher)
	{
		return 0;
	}

	dispatcher->Dispatch(buffer);
	if (outHandle)
	{
		*outHandle = alc->GetZCallHandle(dispatcher);
	}
	
	return 1;
}

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext_Interop::GetZCallHandle(const TCHAR* name)
{
	IZMasterAssemblyLoadContext* alc = IZSharpCLR::Get().GetMasterALC();
	if (!alc)
	{
		return FZCallHandle{};
	}
	
	const IZCallDispatcher* dispatcher = alc->GetZCallDispatcher(name);
	if (!dispatcher)
	{
		return FZCallHandle{};
	}

	return alc->GetZCallHandle(dispatcher);
}
