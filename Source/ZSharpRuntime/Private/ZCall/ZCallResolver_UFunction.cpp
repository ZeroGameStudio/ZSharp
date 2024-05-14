// Copyright Zero Games. All Rights Reserved.


#include "ZCallResolver_UFunction.h"

#include "ZCallDispatcher_UFunction.h"

ZSharp::IZCallDispatcher* ZSharp::FZCallResolver_UFunction::Resolve(const FString& name) const
{
	if (name.StartsWith("uf:/"))
	{
		FString functionName = name.RightChop(4);
		FRegexPattern pattern{ "^[/.+]+\\..+:.+$" };
		FRegexMatcher matcher{ pattern, functionName };
		if (matcher.FindNext())
		{
			return new FZCallDispatcher_UFunction { functionName };
		}
	}

	return nullptr;
}


