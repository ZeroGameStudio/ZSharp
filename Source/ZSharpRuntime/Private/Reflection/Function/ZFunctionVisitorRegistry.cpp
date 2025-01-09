// Copyright Zero Games. All Rights Reserved.

#include "Reflection/Function/ZFunctionVisitorRegistry.h"

#include "CLR/IZSharpClr.h"

const ZSharp::FZFunctionVisitorRegistry& ZSharp::FZFunctionVisitorRegistry::Get()
{
	static const FZFunctionVisitorRegistry GInstance;

	return GInstance;
}

ZSharp::FZFunctionVisitorHandle ZSharp::FZFunctionVisitorRegistry::Get(const UFunction* function) const
{
	if (!function)
	{
		return {};
	}

	const UFunction* ancestor = function;
	while (const UFunction* super = ancestor->GetSuperFunction())
	{
		ancestor = super;
	}

	return { ancestor };
}

ZSharp::FZFunctionVisitorRegistry::FZFunctionVisitorRegistry()
{
	MasterAlcUnloadedDelegate = IZSharpClr::Get().OnMasterAlcUnloaded().AddRaw(this, &ThisClass::ClearAlcSensitiveStates);
}

ZSharp::FZFunctionVisitorRegistry::~FZFunctionVisitorRegistry()
{
	IZSharpClr::Get().OnMasterAlcUnloaded().Remove(MasterAlcUnloadedDelegate);
}

const ZSharp::FZFunctionVisitor* ZSharp::FZFunctionVisitorRegistry::Get(FZFunctionVisitorHandle handle) const
{
	const auto pVisitor = FunctionMap.Find(handle);
	const auto func = Cast<UFunction>(handle.Key.ResolveObjectPtrEvenIfGarbage());
	if (!func)
	{
		if (pVisitor)
		{
			FunctionMap.Remove(handle);
		}
		return nullptr;
	}

	check(!func->GetSuperFunction());

	if (!pVisitor)
	{
		const auto visitor = new FZFunctionVisitor { func };
		if (!visitor->IsAvailable())
		{
			return nullptr;
		}
		FunctionMap.Emplace(handle, visitor);
		return visitor;
	}

	return pVisitor->Get();
}

void ZSharp::FZFunctionVisitorRegistry::ClearAlcSensitiveStates()
{
	FZFunctionVisitor::StaticClearAlcSensitiveStates();
}


