// Copyright Zero Games. All Rights Reserved.


#include "ManagedDelegateProxy.h"

#include "Reflection/Function/ZFunctionVisitorRegistry.h"

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
	
	if (!SignatureFunctionVisitor)
	{
		if (SignatureFunctionVisitor = ZSharp::FZFunctionVisitorRegistry::Get().Get(Signature.Get()); !SignatureFunctionVisitor)
		{
			return;
		}
	}

	SignatureFunctionVisitor->InvokeZCall(this, parms);
}


