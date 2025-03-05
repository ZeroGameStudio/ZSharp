// Copyright Zero Games. All Rights Reserved.

#include "Conjugate/ZStrangeConjugateRegistries.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Conjugate/ZDeclareConjugateRegistryMacros.h"
#include "Reflection/ZReflectionHelper.h"
#include "Reflection/Property/ZPropertyFactory.h"
#include "Trait/ZConjugateKey.h"

ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_Array)
ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_Set)
ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_Map)
ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_Optional)
ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_UScriptStruct)
ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_Delegate)
ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_MulticastInlineDelegate)
ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_MulticastSparseDelegate)

ZSharp::FZSelfDescriptiveScriptArray* ZSharp::FZConjugateRegistry_Array::BuildConjugateWrapper(void* userdata)
{
	struct
	{
		FZPropertyDesc Desc;
	} typedUserdata = *static_cast<decltype(typedUserdata)*>(userdata);
	
	return new FZSelfDescriptiveScriptArray { FZPropertyFactory::Create(typedUserdata.Desc), true };
}

void ZSharp::FZConjugateRegistry_Array::ValidateConjugateWrapper(const FProperty* elementProperty, const FZSelfDescriptiveScriptArray* wrapper)
{
	check(wrapper->GetDescriptor()->GetClass() == elementProperty->GetClass());
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_Array::GetManagedType(const FProperty* elementProperty) const
{
	FString rootKey = FZReflectionHelper::GetFieldClassConjugateKey(FArrayProperty::StaticClass());
	FZRuntimeTypeUri inner = FZReflectionHelper::GetContainerElementRuntimeTypeUriFromProperty(elementProperty);
	FZRuntimeTypeUri uri { rootKey, inner };
	return Alc.GetType(uri);
}

ZSharp::FZSelfDescriptiveScriptSet* ZSharp::FZConjugateRegistry_Set::BuildConjugateWrapper(void* userdata)
{
	struct
	{
		FZPropertyDesc Desc;
	} typedUserdata = *static_cast<decltype(typedUserdata)*>(userdata);
	
	return new FZSelfDescriptiveScriptSet { FZPropertyFactory::Create(typedUserdata.Desc), true };
}

void ZSharp::FZConjugateRegistry_Set::ValidateConjugateWrapper(const FProperty* elementProperty, const FZSelfDescriptiveScriptSet* wrapper)
{
	check(wrapper->GetDescriptor()->GetClass() == elementProperty->GetClass());
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_Set::GetManagedType(const FProperty* elementProperty) const
{
	FString rootKey = FZReflectionHelper::GetFieldClassConjugateKey(FSetProperty::StaticClass());
	FZRuntimeTypeUri inner = FZReflectionHelper::GetContainerElementRuntimeTypeUriFromProperty(elementProperty);
	FZRuntimeTypeUri uri { rootKey, inner };
	return Alc.GetType(uri);
}

ZSharp::FZSelfDescriptiveScriptMap* ZSharp::FZConjugateRegistry_Map::BuildConjugateWrapper(void* userdata)
{
	struct
	{
		FZPropertyDesc KeyDesc;
		FZPropertyDesc ValueDesc;
	} typedUserdata = *static_cast<decltype(typedUserdata)*>(userdata);

	const FProperty* keyProperty = FZPropertyFactory::Create(typedUserdata.KeyDesc);
	FProperty* valueProperty = FZPropertyFactory::Create(typedUserdata.ValueDesc);

	int32 valueOffset = FScriptMap::GetScriptLayout(keyProperty->GetSize(), keyProperty->GetMinAlignment(), valueProperty->GetSize(), valueProperty->GetMinAlignment()).ValueOffset;
	UEProperty_Private::FProperty_DoNotUse::Unsafe_AlterOffset(*valueProperty, valueOffset);
	
	auto desc = new TPair<const FProperty*, const FProperty*> { keyProperty, valueProperty };
	
	return new FZSelfDescriptiveScriptMap { desc, true };
}

void ZSharp::FZConjugateRegistry_Map::ValidateConjugateWrapper(const TPair<const FProperty*, const FProperty*>* descriptor, const FZSelfDescriptiveScriptMap* wrapper)
{
	const FProperty* keyProperty = descriptor->Key;
	const FProperty* valueProperty = descriptor->Value;

	check(wrapper->GetDescriptor()->Key->GetClass() == keyProperty->GetClass());
	check(wrapper->GetDescriptor()->Value->GetClass() == valueProperty->GetClass());
	
	delete descriptor;
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_Map::GetManagedType(const TPair<const FProperty*, const FProperty*>* descriptor) const
{
	FString rootKey = FZReflectionHelper::GetFieldClassConjugateKey(FMapProperty::StaticClass());
	FZRuntimeTypeUri inner = FZReflectionHelper::GetContainerElementRuntimeTypeUriFromProperty(descriptor->Key);
	FZRuntimeTypeUri outer = FZReflectionHelper::GetContainerElementRuntimeTypeUriFromProperty(descriptor->Value);
	FZRuntimeTypeUri uri { rootKey, inner, outer };
	return Alc.GetType(uri);
}

ZSharp::FZSelfDescriptiveOptional* ZSharp::FZConjugateRegistry_Optional::BuildConjugateWrapper(void* userdata)
{
	struct
	{
		FZPropertyDesc Desc;
	} typedUserdata = *static_cast<decltype(typedUserdata)*>(userdata);
	
	return new FZSelfDescriptiveOptional { FZPropertyFactory::Create(typedUserdata.Desc), true };
}

void ZSharp::FZConjugateRegistry_Optional::ValidateConjugateWrapper(const FProperty* elementProperty, const FZSelfDescriptiveOptional* wrapper)
{
	check(wrapper->GetDescriptor()->GetClass() == elementProperty->GetClass());
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_Optional::GetManagedType(const FProperty* elementProperty) const
{
	FString rootKey = FZReflectionHelper::GetFieldClassConjugateKey(FOptionalProperty::StaticClass());
	FZRuntimeTypeUri inner = FZReflectionHelper::GetContainerElementRuntimeTypeUriFromProperty(elementProperty);
	FZRuntimeTypeUri uri { rootKey, inner };
	return Alc.GetType(uri);
}

ZSharp::FZSelfDescriptiveScriptStruct* ZSharp::FZConjugateRegistry_UScriptStruct::BuildConjugateWrapper(void* userdata)
{
	const UScriptStruct* scriptStruct = static_cast<UScriptStruct*>(userdata);
	return new FZSelfDescriptiveScriptStruct { scriptStruct };
}

void ZSharp::FZConjugateRegistry_UScriptStruct::ValidateConjugateWrapper(const UScriptStruct* descriptor, const FZSelfDescriptiveScriptStruct* wrapper)
{
	check(wrapper->GetDescriptor() == descriptor);
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_UScriptStruct::GetManagedType(const UScriptStruct* scriptStruct) const
{
	const UScriptStruct* currentStruct = scriptStruct;
	while (currentStruct)
	{
		if (currentStruct->IsNative())
		{
			FZRuntimeTypeUri uri { FZReflectionHelper::GetFieldConjugateKey(currentStruct) };
			if (FZRuntimeTypeHandle type = Alc.GetType(uri))
			{
				return type;
			}
		}

		currentStruct = CastChecked<UScriptStruct>(currentStruct->GetSuperStruct(), ECastCheckedType::NullAllowed);
	}

	return Alc.GetType(FZRuntimeTypeUri { TZConjugateKey<FZDynamicScriptStruct>::Value });
}

ZSharp::FZSelfDescriptiveScriptDelegate* ZSharp::FZConjugateRegistry_Delegate::BuildConjugateWrapper(void* userdata)
{
	auto signature = static_cast<const UDelegateFunction*>(userdata);
	return new FZSelfDescriptiveScriptDelegate { signature };
}

void ZSharp::FZConjugateRegistry_Delegate::ValidateConjugateWrapper(const UDelegateFunction* descriptor, const FZSelfDescriptiveScriptDelegate* wrapper)
{
	check(wrapper->GetDescriptor() == descriptor);
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_Delegate::GetManagedType(const UDelegateFunction* signature) const
{
	FZRuntimeTypeUri uri { FZReflectionHelper::GetFieldConjugateKey(signature) };
	return Alc.GetType(uri);
}

ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate* ZSharp::FZConjugateRegistry_MulticastInlineDelegate::BuildConjugateWrapper(void* userdata)
{
	auto signature = static_cast<const UDelegateFunction*>(userdata);
	return new FZSelfDescriptiveMulticastInlineScriptDelegate { signature };
}

void ZSharp::FZConjugateRegistry_MulticastInlineDelegate::ValidateConjugateWrapper(const UDelegateFunction* descriptor, const FZSelfDescriptiveMulticastInlineScriptDelegate* wrapper)
{
	check(wrapper->GetDescriptor() == descriptor);
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_MulticastInlineDelegate::GetManagedType(const UDelegateFunction* signature) const
{
	FZRuntimeTypeUri uri { FZReflectionHelper::GetFieldConjugateKey(signature) };
	FZRuntimeTypeHandle type = Alc.GetType(uri);
	return type ? type : Alc.GetType(FZRuntimeTypeUri { TZConjugateKey<FZDynamicMulticastInlineDelegate>::Value });
}

ZSharp::FZSelfDescriptiveMulticastSparseScriptDelegate* ZSharp::FZConjugateRegistry_MulticastSparseDelegate::BuildConjugateWrapper(void* userdata)
{
	// There is no black SparseDelegate conjugate.
	checkNoEntry();
	return nullptr;
}

void ZSharp::FZConjugateRegistry_MulticastSparseDelegate::ValidateConjugateWrapper(const USparseDelegateFunction* descriptor, const FZSelfDescriptiveMulticastSparseScriptDelegate* wrapper)
{
	check(wrapper->GetDescriptor() == descriptor);
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_MulticastSparseDelegate::GetManagedType(const USparseDelegateFunction* signature) const
{
	FZRuntimeTypeUri uri { FZReflectionHelper::GetFieldConjugateKey(signature) };
	return Alc.GetType(uri);
}


