﻿// Copyright Zero Games. All Rights Reserved.

#include "ZDynamicallyExportedMethod.h"

#include "ZDynamicallyExportedParameter.h"
#include "ZExportHelper.h"

ZSharp::FZDynamicallyExportedMethod* ZSharp::FZDynamicallyExportedMethod::Create(const UFunction* function)
{
	if (!FZExportHelper::ShouldExportField(function))
	{
		return nullptr;
	}

	// Skip delegate signatures.
	if (function->HasAnyFunctionFlags(FUNC_Delegate))
	{
		return nullptr;
	}
	
	auto exportedFunction = new FZDynamicallyExportedMethod { function };
	if (!exportedFunction->IsValid())
	{
		delete exportedFunction;
		exportedFunction = nullptr;
	}

	return exportedFunction;
}

FString ZSharp::FZDynamicallyExportedMethod::GetName() const
{
	return FZExportHelper::GetFieldRedirectedName(Function);
}

ZSharp::EZExportedMethodFlags ZSharp::FZDynamicallyExportedMethod::GetFlags() const
{
	return Flags;
}

FString ZSharp::FZDynamicallyExportedMethod::GetZCallName() const
{
	static const TCHAR* GVirtualZCallProtocol = TEXT("uf:/");
	static const TCHAR* GFinalZCallProtocol = TEXT("uf!:/");

	bool isVirtual = (Flags & EZExportedMethodFlags::Virtual) != EZExportedMethodFlags::None;
	return FString::Printf(TEXT("%s%s"), isVirtual ? GVirtualZCallProtocol : GFinalZCallProtocol, *Function->GetPathName());
}

ZSharp::FZFullyExportedTypeName ZSharp::FZDynamicallyExportedMethod::GetOwnerInterface() const
{
	const UClass* owner = Function->GetOwnerClass();
	if (owner->HasAllClassFlags(CLASS_Interface))
	{
		return FZExportHelper::GetFieldFullyExportedTypeName(owner);
	}

	return {};
}

void ZSharp::FZDynamicallyExportedMethod::ForeachParameter(TFunctionRef<void(const IZExportedParameter&)> action) const
{
	for (const auto& param : Parameters)
	{
		action(*param);
	}
}

ZSharp::FZDynamicallyExportedMethod::FZDynamicallyExportedMethod(const UFunction* function)
	: bValid(false)
	, Function(function)
	, Flags(EZExportedMethodFlags::None)
{
	if (Function->GetOwnerClass()->HasAllClassFlags(CLASS_Interface) ||
		Function->HasAllFunctionFlags(FUNC_Public) ||
		Function->GetBoolMetaData("AllowPrivateAccess"))
	{
		Flags |= EZExportedMethodFlags::Public;
	}
	else if (Function->HasAllFunctionFlags(FUNC_Protected))
	{
		Flags |= EZExportedMethodFlags::Protected;
	}
	else if (Function->HasAllFunctionFlags(FUNC_Private))
	{
		Flags |= EZExportedMethodFlags::Private;
	}

	if (Function->HasAllFunctionFlags(FUNC_Static))
	{
		Flags |= EZExportedMethodFlags::Static;
	}
	else if (Function->HasAllFunctionFlags(FUNC_Event))
	{
		Flags |= EZExportedMethodFlags::Virtual;
		if (!Function->HasAllFunctionFlags(FUNC_Native))
		{
			Flags |= EZExportedMethodFlags::Abstract;
		}
	}

	for (TFieldIterator<FProperty> it(Function); it && it->HasAllPropertyFlags(CPF_Parm); ++it)
	{
		FZDynamicallyExportedParameter* param = FZDynamicallyExportedParameter::Create(*it);
		if (!param)
		{
			return;
		}

		Parameters.Emplace(param);
	}

	bValid = true;
}


