// Copyright Zero Games. All Rights Reserved.


#include "ZManagedDelegateProxyImpl.h"

#include "Reflection/Function/ZFunctionVisitorRegistry.h"

const FName UZManagedDelegateProxyImpl::StubFunctionName = GET_FUNCTION_NAME_CHECKED(UZManagedDelegateProxyImpl, __ZStub);

void UZManagedDelegateProxyImpl::BeginDestroy()
{
	Delegate.Free();
	if (State)
	{
		State->Free();
	}
	
	UObject::BeginDestroy();
}

void UZManagedDelegateProxyImpl::ProcessEvent(UFunction* function, void* parms)
{
	if (function->GetName() != GET_FUNCTION_NAME_CHECKED(ThisClass, __ZStub))
	{
		return Super::ProcessEvent(function, parms);
	}
	
	if (!SignatureFunctionVisitor)
	{
		if (SignatureFunctionVisitor = ZSharp::FZFunctionVisitorRegistry::Get().Get(Signature.Get()); !SignatureFunctionVisitor)
		{
			return;
		}
	}

	SignatureFunctionVisitor->InvokeZCall(this, parms);
}


