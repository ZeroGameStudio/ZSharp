// Copyright Zero Games. All Rights Reserved.

#include "ZCallDispatcher_FinalUFunction.h"

#include "Reflection/Function/ZFunctionVisitorRegistry.h"

ZSharp::EZCallErrorCode ZSharp::FZCallDispatcher_FinalUFunction::Dispatch(FZCallBuffer* buffer) const
{
	if ((!FinalFunction.IsValid() || !Function) && !InvalidateCache())
	{
		return EZCallErrorCode::DispatcherError;
	}

	return Function->InvokeUFunction(buffer, FinalFunction.Get());
}

bool ZSharp::FZCallDispatcher_FinalUFunction::InvalidateCache() const
{
	// @FIXME: Can we just find this function since it must have already loaded?
	auto func = LoadObject<UFunction>(nullptr, *Path);
	if (!func)
	{
		return false;
	}

	FinalFunction = func;
	Function = FZFunctionVisitorRegistry::Get().Get(func);
	if (!Function)
	{
		return false;
	}

	return true;
}


