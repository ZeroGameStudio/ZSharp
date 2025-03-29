// Copyright Zero Games. All Rights Reserved.

#include "ZCallDispatcher_FinalUFunction.h"

#include "Reflection/Function/ZFunctionVisitorRegistry.h"

ZSharp::EZCallErrorCode ZSharp::FZCallDispatcher_FinalUFunction::Dispatch(FZCallBuffer* buffer) const
{
	if (!NativeFunction && !Function && !InvalidateCache())
	{
		return EZCallErrorCode::DispatcherError;
	}

	if (!bNativeFinalFunction && !FinalFunction.IsValid() && !ResolveFinalFunction())
	{
		return EZCallErrorCode::DispatcherError;
	}

	return NativeFunction ? NativeFunction->InvokeUFunction(buffer, FinalFunction.Get()) : Function->InvokeUFunction(buffer, FinalFunction.Get());
}

bool ZSharp::FZCallDispatcher_FinalUFunction::ResolveFinalFunction() const
{
	auto func = LoadObject<UFunction>(nullptr, *Path);
	if (!func)
	{
		return false;
	}

	FinalFunction = func;
	bNativeFinalFunction = func->IsNative();
	return true;
}

bool ZSharp::FZCallDispatcher_FinalUFunction::InvalidateCache() const
{
	if (!ResolveFinalFunction())
	{
		return false;
	}

	bool native;
	Function = FZFunctionVisitorRegistry::Get().Get(FinalFunction.Get(), &native);
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


