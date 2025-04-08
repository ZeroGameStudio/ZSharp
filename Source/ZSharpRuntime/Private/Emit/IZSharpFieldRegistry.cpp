// Copyright Zero Games. All Rights Reserved.

#include "Emit/IZSharpFieldRegistry.h"

#include "ZSharpFieldRegistry.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"

ZSharp::FZCallHandle ZSharp::FZSharpScriptStruct::GetNetSerializeZCallHandle() const
{
	check(!NetSerializeZCallName.IsEmpty());
	
	if (!NetSerializeZCallHandle)
	{
		if (IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc())
		{
			NetSerializeZCallHandle = alc->GetZCallHandle(NetSerializeZCallName);
		}
	}

	check(NetSerializeZCallHandle);
	return NetSerializeZCallHandle;
}

ZSharp::FZCallHandle ZSharp::FZSharpClass::GetConstructorZCallHandle() const
{
	check(bConstructor);
	check(!ConstructorZCallName.IsEmpty());
	
	if (!ConstructorZCallHandle)
	{
		if (IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc())
		{
			ConstructorZCallHandle = alc->GetZCallHandle(ConstructorZCallName);
		}
	}

	check(ConstructorZCallHandle);
	return ConstructorZCallHandle;
}

ZSharp::FZCallHandle ZSharp::FZSharpFunction::GetZCallHandle() const
{
	if (!ZCallHandle)
	{
		if (IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc())
		{
			ZCallHandle = alc->GetZCallHandle(ZCallName);
		}
	}

	check(ZCallHandle);
	return ZCallHandle;
}

ZSharp::FZCallHandle ZSharp::FZSharpFunction::GetValidateZCallHandle() const
{
	if (!ValidateZCallHandle)
	{
		if (IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc())
		{
			ValidateZCallHandle = alc->GetZCallHandle(ValidateZCallName);
		}
	}

	check(ValidateZCallHandle);
	return ValidateZCallHandle;
}

ZSharp::IZSharpFieldRegistry& ZSharp::IZSharpFieldRegistry::Get()
{
	return FZSharpFieldRegistry::Get();
}


