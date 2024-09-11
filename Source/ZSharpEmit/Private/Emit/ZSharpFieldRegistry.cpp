// Copyright Zero Games. All Rights Reserved.


#include "ZSharpFieldRegistry.h"

ZSharp::FZSharpFieldRegistry& ZSharp::FZSharpFieldRegistry::Get()
{
	static FZSharpFieldRegistry GSingleton;

	return GSingleton;
}

const ZSharp::FZSharpClass* ZSharp::FZSharpFieldRegistry::GetClass(const UClass* cls) const
{
	const TUniquePtr<FZSharpClass>* pCls = ClassRegistry.Find(cls);
	return pCls ? pCls->Get() : nullptr;
}

const ZSharp::FZSharpFunction* ZSharp::FZSharpFieldRegistry::GetFunction(const UFunction* function) const
{
	const TUniquePtr<FZSharpFunction>* pFunction = FunctionRegistry.Find(function);
	return pFunction ? pFunction->Get() : nullptr;
}

ZSharp::FZSharpClass* ZSharp::FZSharpFieldRegistry::RegisterClass(const UClass* cls)
{
	FZSharpClass* zscls = new FZSharpClass;
	return ClassRegistry.Emplace(cls, zscls).Get();
}

ZSharp::FZSharpFunction* ZSharp::FZSharpFieldRegistry::RegisterFunction(const UFunction* function)
{
	FZSharpFunction* zsfunction = new FZSharpFunction;
	return FunctionRegistry.Emplace(function, zsfunction).Get();
}


