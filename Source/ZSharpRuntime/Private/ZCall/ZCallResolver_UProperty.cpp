// Copyright Zero Games. All Rights Reserved.


#include "ZCallResolver_UProperty.h"

#include "ZCallDispatcher_UProperty.h"

ZSharp::IZCallDispatcher* ZSharp::FZCallResolver_UProperty::Resolve(const FString& name) const
{
	if (name.StartsWith("up:/"))
	{
		FString propertyName = name.RightChop(4);
		FRegexPattern pattern{ "^(/.+)+\\..+:.+$" };
		FRegexMatcher matcher{ pattern, propertyName };
		if (matcher.FindNext())
		{
			return new FZCallDispatcher_UProperty { name };
		}
	}

	return nullptr;
}


