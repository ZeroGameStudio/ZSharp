// Copyright Zero Games. All Rights Reserved.


#include "ZDynamicallyExportedDelegate.h"

#include "ZExportedTypeRegistry.h"
#include "ZDynamicallyExportedParameter.h"
#include "ZExportHelper.h"
#include "Reflection/ZReflectionHelper.h"

ZSharp::FZDynamicallyExportedDelegate* ZSharp::FZDynamicallyExportedDelegate::Create(const UDelegateFunction* signature)
{
	if (!FZExportHelper::ShouldExportField(signature))
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
	return FZExportHelper::GetFieldRedirectedFullName(Signature);
}

FString ZSharp::FZDynamicallyExportedDelegate::GetModule() const
{
	return FZExportHelper::GetFieldModuleName(Signature);
}

FString ZSharp::FZDynamicallyExportedDelegate::GetUnrealFieldPath() const
{
	return Signature->GetPathName();
}

FString ZSharp::FZDynamicallyExportedDelegate::GetConjugateKey() const
{
	return FZReflectionHelper::GetFieldConjugateKey(Signature);
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

ZSharp::FZDynamicallyExportedDelegate::FZDynamicallyExportedDelegate(const UDelegateFunction* signature)
	: bValid(false)
	, Signature(signature)
	, Flags(EZExportedDelegateFlags::None)
{
	if (Signature->HasAllFunctionFlags(FUNC_MulticastDelegate))
	{
		Flags |= EZExportedDelegateFlags::Multicast;
	}

	if (Signature->IsA<USparseDelegateFunction>())
	{
		check(HasAllFlags(EZExportedDelegateFlags::Multicast));
		Flags |= EZExportedDelegateFlags::Sparse;
	}

	for (TFieldIterator<FProperty> it(Signature); it && it->HasAllPropertyFlags(CPF_Parm); ++it)
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


