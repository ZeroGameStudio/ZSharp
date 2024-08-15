// Copyright Zero Games. All Rights Reserved.


#include "ZMasterAssemblyLoadContext_Interop.h"

#include "ZSehHelper.h"
#include "ALC/ZMasterAssemblyLoadContext.h"
#include "CLR/ZGenericClr.h"

ZSharp::EZCallErrorCode ZSharp::FZMasterAssemblyLoadContext_Interop::ZCall_Black(FZCallHandle handle, FZCallBuffer* buffer)
{
	auto alc = static_cast<FZMasterAssemblyLoadContext*>(FZGenericClr::Get().GetMasterAlc());
	if (!alc)
	{
		return EZCallErrorCode::AlcUnavailable;
	}

	GUARDED_INVOKE(alc->ZCall_Black(handle, buffer), EZCallErrorCode::UnknownError);
}

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext_Interop::GetZCallHandle_Black(const TCHAR* name)
{
	auto alc = static_cast<FZMasterAssemblyLoadContext*>(FZGenericClr::Get().GetMasterAlc());
	if (!alc)
	{
		return FZCallHandle{};
	}

	return alc->GetZCallHandle_Black({ name });
}

void* ZSharp::FZMasterAssemblyLoadContext_Interop::BuildConjugate_Black(uint16 registryId, void* userdata)
{
	auto alc = static_cast<FZMasterAssemblyLoadContext*>(FZGenericClr::Get().GetMasterAlc());
	if (!alc)
	{
		return nullptr;
	}

	return alc->BuildConjugate_Black(registryId, userdata);
}

void ZSharp::FZMasterAssemblyLoadContext_Interop::ReleaseConjugate_Black(uint16 registryId, void* unmanaged)
{
	auto alc = static_cast<FZMasterAssemblyLoadContext*>(FZGenericClr::Get().GetMasterAlc());
	if (!alc)
	{
		return;
	}

	return alc->ReleaseConjugate_Black(registryId, unmanaged);
}


