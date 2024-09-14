// Copyright Zero Games. All Rights Reserved.


#include "Emit/IZSharpFieldRegistry.h"

#include "ZSharpFieldRegistry.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"

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

ZSharp::IZSharpFieldRegistry& ZSharp::IZSharpFieldRegistry::Get()
{
	return FZSharpFieldRegistry::Get();
}


