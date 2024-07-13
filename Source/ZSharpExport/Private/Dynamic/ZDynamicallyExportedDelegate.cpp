// Copyright Zero Games. All Rights Reserved.


#include "Dynamic/ZDynamicallyExportedDelegate.h"

#include "ZExportedTypeRegistry.h"
#include "Dynamic/ZDynamicallyExportedParameter.h"
#include "Reflection/ZReflectionHelper.h"

ZSharp::FZDynamicallyExportedDelegate* ZSharp::FZDynamicallyExportedDelegate::Create(const UFunction* signature)
{
	if (!signature->HasAnyFunctionFlags(FUNC_Delegate))
	{
		return nullptr;
	}
	
	auto delegate = new FZDynamicallyExportedDelegate { signature };
	if (!delegate->IsValid() || !FZExportedTypeRegistry::Get().RegisterDelegate(delegate))
	{
		delete delegate;
		delegate = nullptr;
	}

	return delegate;
}

FString ZSharp::FZDynamicallyExportedDelegate::GetName() const
{
	return FZReflectionHelper::GetUFieldAliasedName(Signature);
}

FString ZSharp::FZDynamicallyExportedDelegate::GetModule() const
{
	return FZReflectionHelper::GetUFieldModuleName(Signature);
}

FString ZSharp::FZDynamicallyExportedDelegate::GetUnrealFieldPath() const
{
	return Signature->GetPathName();
}

ZSharp::EZExportedDelegateFlags ZSharp::FZDynamicallyExportedDelegate::GetFlags() const
{
	return Flags;
}

void ZSharp::FZDynamicallyExportedDelegate::ForeachParameter(TFunctionRef<void(const IZExportedParameter&)> action) const
{
	for (const auto& param : Parameters)
	{
		action(*param);
	}
}

ZSharp::FZDynamicallyExportedDelegate::FZDynamicallyExportedDelegate(const UFunction* signature)
	: bValid(false)
	, Signature(signature)
	, Flags(EZExportedDelegateFlags::None)
{
	check(Signature->HasAllFunctionFlags(FUNC_Delegate));
	if (Signature->HasAllFunctionFlags(FUNC_MulticastDelegate))
	{
		Flags |= EZExportedDelegateFlags::Multicast;
	}

	for (TFieldIterator<FProperty> it(Signature); it; ++it)
	{
		FProperty* property = *it;
		check(property->ArrayDim == 1);
		FZDynamicallyExportedParameter* param = FZDynamicallyExportedParameter::Create(property);
		if (!param)
		{
			return;
		}

		Parameters.Emplace(param);
	}

	bValid = true;
}


