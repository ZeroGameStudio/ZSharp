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
	if (!Function && !InvalidateCache())
	{
		return EZCallErrorCode::DispatcherError;
	}

	return Function->InvokeUFunction(buffer);
}

bool ZSharp::FZCallDispatcher_VirtualUFunction::InvalidateCache() const
{
	// @FIXME: Can we just find this function since it must have already loaded?
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

	Function = FZFunctionVisitorRegistry::Get().Get(func);
	if (!Function)
	{
		return false;
	}

	return true;
}


