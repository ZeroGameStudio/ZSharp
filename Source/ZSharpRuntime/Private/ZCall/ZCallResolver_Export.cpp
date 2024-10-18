// Copyright Zero Games. All Rights Reserved.


#include "ZCallResolver_Export.h"

#include "ZCallDispatcher_Export.h"

namespace ZSharp::ZCallResolver_Export_Private
{
	TMap<FString, TFunction<IZCallDispatcher*()>>& GetResolveFunctionMap()
	{
		static TMap<FString, TFunction<IZCallDispatcher*()>> GInstance;

		return GInstance;
	}

}

void ZSharp::FZCallResolver_Export::RegisterFunction(const FString& name, const TFunction<EZCallErrorCode(FZCallBuffer*)>& function)
{
	check(!ZCallResolver_Export_Private::GetResolveFunctionMap().Contains(name));
	ZCallResolver_Export_Private::GetResolveFunctionMap().Emplace(name, [name, function]
	{
		return new FZCallDispatcher_Export { name, function };
	});
}

ZSharp::IZCallDispatcher* ZSharp::FZCallResolver_Export::Resolve(const FString& name) const
{
	const TFunction<IZCallDispatcher*()>* resolveFunction = ZCallResolver_Export_Private::GetResolveFunctionMap().Find(name);
	if (resolveFunction)
	{
		return (*resolveFunction)();
	}

	return nullptr;
}


