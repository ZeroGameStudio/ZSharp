// Copyright Zero Games. All Rights Reserved.


#include "ZCallResolver_Export.h"

#include "ZCallDispatcher_Export.h"

namespace ZSharp::ZCallResolver_Export_Private
{
	static TMap<FString, TFunction<IZCallDispatcher*()>> GResolveFunctionMap;
}

void ZSharp::FZCallResolver_Export::RegisterFunction(const FString& name, const TFunction<int32(FZCallBuffer*)>& function)
{
	ZCallResolver_Export_Private::GResolveFunctionMap.Emplace(name, [name, function]
	{
		return new FZCallDispatcher_Export { name, function };
	});
}

ZSharp::IZCallDispatcher* ZSharp::FZCallResolver_Export::Resolve(const FString& name) const
{
	const TFunction<IZCallDispatcher*()>* resolveFunction = ZCallResolver_Export_Private::GResolveFunctionMap.Find(name);
	if (resolveFunction)
	{
		return (*resolveFunction)();
	}

	return nullptr;
}


