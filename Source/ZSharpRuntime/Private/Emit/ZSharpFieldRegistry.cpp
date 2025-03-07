// Copyright Zero Games. All Rights Reserved.

#include "ZSharpFieldRegistry.h"

#include "CLR/IZSharpClr.h"

ZSharp::FZSharpFieldRegistry& ZSharp::FZSharpFieldRegistry::Get()
{
	static FZSharpFieldRegistry GInstance;

	return GInstance;
}

const ZSharp::FZSharpEnum* ZSharp::FZSharpFieldRegistry::GetEnum(const UEnum* enm) const
{
	const TUniquePtr<FZSharpEnum>* pEnm = EnumRegistry.Find(enm);
	return pEnm ? pEnm->Get() : nullptr;
}

const ZSharp::FZSharpStruct* ZSharp::FZSharpFieldRegistry::GetStruct(const UScriptStruct* scriptStruct) const
{
	const TUniquePtr<FZSharpStruct>* pStruct = StructRegistry.Find(scriptStruct);
	return pStruct ? pStruct->Get() : nullptr;
}

const ZSharp::FZSharpDelegate* ZSharp::FZSharpFieldRegistry::GetDelegate(const UDelegateFunction* delegate) const
{
	const TUniquePtr<FZSharpDelegate>* pDelegate = DelegateRegistry.Find(delegate);
	return pDelegate ? pDelegate->Get() : nullptr;
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

ZSharp::FZSharpEnum& ZSharp::FZSharpFieldRegistry::RegisterEnum(const UEnum* enm)
{
	auto zsenm = new FZSharpEnum { enm };
	return *EnumRegistry.Emplace(enm, zsenm);
}

ZSharp::FZSharpStruct& ZSharp::FZSharpFieldRegistry::RegisterStruct(const UScriptStruct* scriptStruct)
{
	auto zsstruct = new FZSharpStruct { scriptStruct };
	return *StructRegistry.Emplace(scriptStruct, zsstruct);
}

ZSharp::FZSharpDelegate& ZSharp::FZSharpFieldRegistry::RegisterDelegate(const UDelegateFunction* delegate)
{
	auto zsdelegate = new FZSharpDelegate { delegate };
	return *DelegateRegistry.Emplace(delegate, zsdelegate);
}

ZSharp::FZSharpClass& ZSharp::FZSharpFieldRegistry::RegisterClass(const UClass* cls)
{
	auto zscls = new FZSharpClass;
	zscls->Class = cls;
	return *ClassRegistry.Emplace(cls, zscls);
}

ZSharp::FZSharpFunction& ZSharp::FZSharpFieldRegistry::RegisterFunction(const UFunction* function)
{
	auto zsfunction = new FZSharpFunction;
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
	for (const auto& pair : ClassRegistry)
	{
		pair.Value->ConstructorZCallHandle.Reset();
	}
	
	for (const auto& pair : FunctionRegistry)
	{
		pair.Value->ZCallHandle = {};
		pair.Value->ValidateZCallHandle = {};
	}
}


