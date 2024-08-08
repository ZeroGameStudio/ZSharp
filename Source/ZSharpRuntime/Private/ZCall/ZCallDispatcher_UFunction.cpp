// Copyright Zero Games. All Rights Reserved.


#include "ZCallDispatcher_UFunction.h"

#include "Reflection/Function/ZFunctionVisitorRegistry.h"
#include "ZCall/ZCallBuffer.h"
#include "ZCall/ZCallBufferSlotEncoder.h"

ZSharp::EZCallErrorCode ZSharp::FZCallDispatcher_UFunction::Dispatch(FZCallBuffer* buffer) const
{
	if (!Function && !InvalidateCache())
	{
		return EZCallErrorCode::DispatcherError;
	}

	return Function->InvokeUFunction(buffer);
}

bool ZSharp::FZCallDispatcher_UFunction::InvalidateCache() const
{
	// @FIXME: Can we just find this function since it must have already loaded?
	const UFunction* func = LoadObject<UFunction>(nullptr, *Path);
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


