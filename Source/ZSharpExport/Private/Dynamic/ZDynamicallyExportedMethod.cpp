// Copyright Zero Games. All Rights Reserved.


#include "Dynamic/ZDynamicallyExportedMethod.h"

#include "Dynamic/ZDynamicallyExportedParameter.h"
#include "Reflection/ZReflectionHelper.h"

ZSharp::FZDynamicallyExportedMethod* ZSharp::FZDynamicallyExportedMethod::Create(const UFunction* function)
{
	if (!function->IsNative())
	{
		return nullptr;
	}

	// Skip delegate signatures.
	if (function->HasAnyFunctionFlags(FUNC_Delegate))
	{
		return nullptr;
	}

	// ZCallDispatcher will handle polymorphism so skip override functions.
	if (function->GetSuperFunction())
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
	return FZReflectionHelper::GetUFieldAliasedName(Function, true);
}

ZSharp::EZExportedMethodFlags ZSharp::FZDynamicallyExportedMethod::GetFlags() const
{
	return Flags;
}

FString ZSharp::FZDynamicallyExportedMethod::GetZCallName() const
{
	return FString::Printf(TEXT("uf:/%s"), *Function->GetPathName());
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
	if (Function->HasAllFunctionFlags(FUNC_Public) || Function->GetBoolMetaData("AllowPrivateAccess"))
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


