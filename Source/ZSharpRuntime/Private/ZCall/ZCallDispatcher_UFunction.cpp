// Copyright Zero Games. All Rights Reserved.


#include "ZCallDispatcher_UFunction.h"

int32 ZSharp::FZCallDispatcher_UFunction::Dispatch(FZCallBuffer* buffer) const
{
	if (!Function.IsValid(true))
	{
		Function = LoadObject<UFunction>(nullptr, *Path);
	}

	if (Function.IsValid())
	{
		
		
		return 0;
	}

	return 1;
}
