// Copyright Zero Games. All Rights Reserved.

#include "ZSharpFieldRegistry.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
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

const ZSharp::FZSharpScriptStruct* ZSharp::FZSharpFieldRegistry::GetScriptStruct(const UScriptStruct* scriptStruct) const
{
	const TUniquePtr<FZSharpScriptStruct>* pStruct = ScriptStructRegistry.Find(scriptStruct);
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

ZSharp::FZSharpScriptStruct& ZSharp::FZSharpFieldRegistry::RegisterScriptStruct(const UScriptStruct* scriptStruct)
{
	auto zsstruct = new FZSharpScriptStruct;
	zsstruct->ScriptStruct = scriptStruct;
	return *ScriptStructRegistry.Emplace(scriptStruct, zsstruct);
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

ZSharp::FZCallHandle ZSharp::FZSharpFieldRegistry::GetObjectPostInitPropertiesZCallHandle() const
{
	static const FString GObjectPostInitPropertiesZCallName = "nm://Script/CoreUObject.Object:.pip";
	
	if (!ObjectPostInitPropertiesZCallHandle)
	{
		if (IZMasterAssemblyLoadContext* alc = IZSharpClr::Get().GetMasterAlc())
		{
			ObjectPostInitPropertiesZCallHandle = alc->GetZCallHandle(GObjectPostInitPropertiesZCallName);
		}
	}

	check(ObjectPostInitPropertiesZCallHandle);
	return ObjectPostInitPropertiesZCallHandle;
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
	ObjectPostInitPropertiesZCallHandle = {};
	
	for (const auto& pair : ScriptStructRegistry)
	{
		pair.Value->NetSerializeZCallHandle = {};
	}
	
	for (const auto& pair : ClassRegistry)
	{
		pair.Value->ConstructorZCallHandle = {};
	}
	
	for (const auto& pair : FunctionRegistry)
	{
		pair.Value->ZCallHandle = {};
		pair.Value->ValidateZCallHandle = {};
	}
}


