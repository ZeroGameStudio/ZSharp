// Copyright Zero Games. All Rights Reserved.

#include "ZMasterAssemblyLoadContext_Interop.h"

#include "ALC/ZMasterAssemblyLoadContext.h"
#include "CLR/ZGenericClr.h"
#include "Interop/ZInteropExceptionHelper.h"

namespace ZSharp::ZMasterAssemblyLoadContext_Interop_Private
{
	static FZCallHandle GetZCallHandle_Black(const TCHAR* name)
	{
		auto alc = static_cast<FZMasterAssemblyLoadContext*>(FZGenericClr::Get().GetMasterAlc());
		if (!alc)
		{
			return FZCallHandle{};
		}

		return alc->GetZCallHandle_Black({ name });
	}
}

ZSharp::EZCallErrorCode ZSharp::FZMasterAssemblyLoadContext_Interop::ZCall_Black(FZCallHandle handle, FZCallBuffer* buffer)
{
	TRY
	{
		auto alc = static_cast<FZMasterAssemblyLoadContext*>(FZGenericClr::Get().GetMasterAlc());
		if (!alc)
		{
			return EZCallErrorCode::AlcUnavailable;
		}

		return alc->ZCall_Black(handle, buffer);
	}
	CATCHR(EZCallErrorCode::UnknownError)
}

ZSharp::FZCallHandle ZSharp::FZMasterAssemblyLoadContext_Interop::GetZCallHandle_Black(const TCHAR* name)
{
	TRY
	{
		return ZMasterAssemblyLoadContext_Interop_Private::GetZCallHandle_Black(name);
	}
	CATCHR({})
}

void* ZSharp::FZMasterAssemblyLoadContext_Interop::BuildConjugate_Black(uint16 registryId, void* userdata)
{
	TRY
	{
		auto alc = static_cast<FZMasterAssemblyLoadContext*>(FZGenericClr::Get().GetMasterAlc());
		if (!alc)
		{
			return nullptr;
		}

		return alc->BuildConjugate_Black(registryId, userdata);
	}
	CATCHR(nullptr)
}

void ZSharp::FZMasterAssemblyLoadContext_Interop::ReleaseConjugate_Black(uint16 registryId, void* unmanaged)
{
	GUARD
	(
		auto alc = static_cast<FZMasterAssemblyLoadContext*>(FZGenericClr::Get().GetMasterAlc());
		if (!alc)
		{
			return;
		}

		return alc->ReleaseConjugate_Black(registryId, unmanaged);
	);
}


