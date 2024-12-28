// Copyright Zero Games. All Rights Reserved.

#include "ALC/ZRuntimeTypeUri.h"

ZSharp::FZRuntimeTypeUri::FZRuntimeTypeUri(const FString& rootKey)
{
	Uri = rootKey;
}

ZSharp::FZRuntimeTypeUri::FZRuntimeTypeUri(const FString& rootKey, const FZRuntimeTypeUri& inner)
{
	if (!rootKey.IsEmpty() && inner)
	{
		Uri = FString::Printf(TEXT("%s<%s>"), *rootKey, **inner);
	}
	else
	{
		Uri = rootKey;
	}
}

ZSharp::FZRuntimeTypeUri::FZRuntimeTypeUri(const FString& rootKey, const FZRuntimeTypeUri& inner, const FZRuntimeTypeUri& outer)
{
	if (!rootKey.IsEmpty() && inner && outer)
	{
		Uri = FString::Printf(TEXT("%s<%s,%s>"), *rootKey, **inner, **outer);
	}
	else
	{
		Uri = rootKey;
	}
}


