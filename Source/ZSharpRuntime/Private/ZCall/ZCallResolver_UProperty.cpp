// Copyright Zero Games. All Rights Reserved.

#include "ZCallResolver_UProperty.h"

#include "ZCallDispatcher_UProperty.h"

ZSharp::IZCallDispatcher* ZSharp::FZCallResolver_UProperty::Resolve(const FString& name) const
{
	static const FRegexPattern GPattern { "^(/.+)+\\..+:.+$" };
	
	if (name.StartsWith("up:/"))
	{
		FString propertyName = name.RightChop(4);
		FRegexMatcher matcher { GPattern, propertyName };
		if (matcher.FindNext())
		{
			return new FZCallDispatcher_UProperty { name };
		}
	}

	return nullptr;
}


