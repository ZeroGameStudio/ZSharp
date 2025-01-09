// Copyright Zero Games. All Rights Reserved.

#include "ZCallResolver_UFunction.h"

#include "ZCallDispatcher_FinalUFunction.h"
#include "ZCallDispatcher_VirtualUFunction.h"

ZSharp::IZCallDispatcher* ZSharp::FZCallResolver_UFunction::Resolve(const FString& name) const
{
	static const FRegexPattern GPattern { "^(/.+)+\\..+:.+$" };
	
	if (name.StartsWith("uf:/"))
	{
		FString functionName = name.RightChop(4);
		FRegexMatcher matcher { GPattern , functionName };
		if (matcher.FindNext())
		{
			return new FZCallDispatcher_VirtualUFunction { name };
		}
	}

	if (name.StartsWith("uf!:/"))
	{
		FString functionName = name.RightChop(5);
		FRegexMatcher matcher { GPattern , functionName };
		if (matcher.FindNext())
		{
			return new FZCallDispatcher_FinalUFunction { name };
		}
	}

	return nullptr;
}


