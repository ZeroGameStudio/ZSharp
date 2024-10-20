// Copyright Zero Games. All Rights Reserved.


#include "ZSharpFieldRegistry.h"

#include "CLR/IZSharpClr.h"

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

ZSharp::FZSharpClass& ZSharp::FZSharpFieldRegistry::RegisterClass(const UClass* cls)
{
	FZSharpClass* zscls = new FZSharpClass { cls };
	return *ClassRegistry.Emplace(cls, zscls);
}

ZSharp::FZSharpFunction& ZSharp::FZSharpFieldRegistry::RegisterFunction(const UFunction* function)
{
	FZSharpFunction* zsfunction = new FZSharpFunction;
	zsfunction->Function = function;
	return *FunctionRegistry.Emplace(function, zsfunction);
}

ZSharp::FZSharpFieldRegistry::FZSharpFieldRegistry()
{
	MasterAlcUnloadedDelegate = IZSharpClr::Get().OnMasterAlcUnloaded().AddRaw(this, &ThisClass::ClearAlcSensitiveStates);
}

ZSharp::FZSharpFieldRegistry::~FZSharpFieldRegistry()
{
	IZSharpClr::Get().OnMasterAlcUnloaded().Remove(MasterAlcUnloadedDelegate);
}

void ZSharp::FZSharpFieldRegistry::ClearAlcSensitiveStates()
{
	for (const auto& pair : FunctionRegistry)
	{
		pair.Value->ZCallHandle = {};
	}
}


