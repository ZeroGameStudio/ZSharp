// Copyright Zero Games. All Rights Reserved.


#include "ManagedDelegateProxy.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "CLR/IZSharpClr.h"
#include "Reflection/Function/ZFunctionVisitorRegistry.h"
#include "ZCall/ZCallBuffer.h"

void UManagedDelegateProxy::BeginDestroy()
{
	Delegate.Free();
	
	UObject::BeginDestroy();
}

void UManagedDelegateProxy::ProcessEvent(UFunction* function, void* parms)
{
	if (function->GetName() != GET_FUNCTION_NAME_CHECKED(ThisClass, __ZStub))
	{
		return Super::ProcessEvent(function, parms);
	}

	if (!DelegateZCallHandle)
	{
		if (ZSharp::IZMasterAssemblyLoadContext* alc = ZSharp::IZSharpClr::Get().GetMasterAlc())
		{
			if (DelegateZCallHandle = alc->GetZCallHandle("d://"); !DelegateZCallHandle)
			{
				return;
			}
		}
		else
		{
			return;
		}
	}
	
	if (!SignatureFunctionVisitor)
	{
		if (SignatureFunctionVisitor = ZSharp::FZFunctionVisitorRegistry::Get().Get(Signature.Get()); !SignatureFunctionVisitor)
		{
			return;
		}
	}

	SignatureFunctionVisitor->InvokeZCall(DelegateZCallHandle, this, parms);
}


