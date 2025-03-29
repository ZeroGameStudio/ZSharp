// Copyright Zero Games. All Rights Reserved.

#include "ZCallDispatcher_VirtualUFunction.h"

#include "Reflection/Function/ZFunctionVisitorRegistry.h"

ZSharp::FZCallDispatcher_VirtualUFunction::FZCallDispatcher_VirtualUFunction(const FString& name)
	: Name(name)
{
	FString functionName;
	Name.RightChop(4).Split(TEXT(":"), &ClassPath, &functionName, ESearchCase::CaseSensitive, ESearchDir::FromEnd);
	FunctionName = FName { functionName };
}

ZSharp::EZCallErrorCode ZSharp::FZCallDispatcher_VirtualUFunction::Dispatch(FZCallBuffer* buffer) const
{
	if (!NativeFunction && !Function && !InvalidateCache())
	{
		return EZCallErrorCode::DispatcherError;
	}

	return NativeFunction ? NativeFunction->InvokeUFunction(buffer) : Function->InvokeUFunction(buffer);
}

bool ZSharp::FZCallDispatcher_VirtualUFunction::InvalidateCache() const
{
	const UClass* cls = LoadObject<UClass>(nullptr, *ClassPath);
	if (!cls)
	{
		return false;
	}
	
	const UFunction* func = cls->FindFunctionByName(FunctionName);
	if (!func)
	{
		return false;
	}

	bool native;
	Function = FZFunctionVisitorRegistry::Get().Get(func, &native);
	if (!Function)
	{
		return false;
	}

	// Native field is never GCed so we can safely cache a pointer.
	if (native)
	{
		NativeFunction = Function.Get();
	}

	return true;
}


