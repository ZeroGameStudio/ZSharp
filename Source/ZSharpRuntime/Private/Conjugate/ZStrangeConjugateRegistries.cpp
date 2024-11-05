// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZStrangeConjugateRegistries.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Conjugate/ZDeclareConjugateRegistryMacros.h"
#include "Reflection/ZReflectionHelper.h"
#include "Reflection/Property/ZPropertyFactory.h"

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
	FZRuntimeTypeUri uri;
	if (!FZReflectionHelper::GetFFieldClassRuntimeTypeLocator(FArrayProperty::StaticClass(), uri))
	{
		return {};
	}
	
	FZRuntimeTypeUri& inner = uri.TypeParameters.AddDefaulted_GetRef();
	if (!FZReflectionHelper::GetNonContainerFPropertyRuntimeTypeLocator(elementProperty, inner))
	{
		return {};
	}
	
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
	FZRuntimeTypeUri uri;
	if (!FZReflectionHelper::GetFFieldClassRuntimeTypeLocator(FSetProperty::StaticClass(), uri))
	{
		return {};
	}
	
	FZRuntimeTypeUri& inner = uri.TypeParameters.AddDefaulted_GetRef();
	if (!FZReflectionHelper::GetNonContainerFPropertyRuntimeTypeLocator(elementProperty, inner))
	{
		return {};
	}
	
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
	const FProperty* keyProperty = descriptor->Key;
	const FProperty* valueProperty = descriptor->Value;
	
	FZRuntimeTypeUri uri;
	if (!FZReflectionHelper::GetFFieldClassRuntimeTypeLocator(FMapProperty::StaticClass(), uri))
	{
		return {};
	}
	
	FZRuntimeTypeUri& inner = uri.TypeParameters.AddDefaulted_GetRef();
	if (!FZReflectionHelper::GetNonContainerFPropertyRuntimeTypeLocator(keyProperty, inner))
	{
		return {};
	}

	FZRuntimeTypeUri& outer = uri.TypeParameters.AddDefaulted_GetRef();
	if (!FZReflectionHelper::GetNonContainerFPropertyRuntimeTypeLocator(valueProperty, outer))
	{
		return {};
	}
	
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
	FZRuntimeTypeUri uri;
	if (!FZReflectionHelper::GetFFieldClassRuntimeTypeLocator(FOptionalProperty::StaticClass(), uri))
	{
		return {};
	}
	
	FZRuntimeTypeUri& inner = uri.TypeParameters.AddDefaulted_GetRef();
	if (!FZReflectionHelper::GetNonContainerFPropertyRuntimeTypeLocator(elementProperty, inner))
	{
		return {};
	}
	
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
	FZRuntimeTypeUri uri;
	if (!FZReflectionHelper::GetUFieldRuntimeTypeLocator(scriptStruct, uri))
	{
		return {};
	}
	
	return Alc.GetType(uri);
}

ZSharp::FZSelfDescriptiveScriptDelegate* ZSharp::FZConjugateRegistry_Delegate::BuildConjugateWrapper(void* userdata)
{
	const auto signature = static_cast<UDelegateFunction*>(userdata);
	return new FZSelfDescriptiveScriptDelegate { signature };
}

void ZSharp::FZConjugateRegistry_Delegate::ValidateConjugateWrapper(const UDelegateFunction* descriptor, const FZSelfDescriptiveScriptDelegate* wrapper)
{
	check(wrapper->GetDescriptor() == descriptor);
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_Delegate::GetManagedType(const UDelegateFunction* signature) const
{
	FZRuntimeTypeUri uri;
	if (!FZReflectionHelper::GetUFieldRuntimeTypeLocator(signature, uri))
	{
		return {};
	}
	
	return Alc.GetType(uri);
}

ZSharp::FZSelfDescriptiveMulticastInlineScriptDelegate* ZSharp::FZConjugateRegistry_MulticastInlineDelegate::BuildConjugateWrapper(void* userdata)
{
	const auto signature = static_cast<UDelegateFunction*>(userdata);
	return new FZSelfDescriptiveMulticastInlineScriptDelegate { signature };
}

void ZSharp::FZConjugateRegistry_MulticastInlineDelegate::ValidateConjugateWrapper(const UDelegateFunction* descriptor, const FZSelfDescriptiveMulticastInlineScriptDelegate* wrapper)
{
	check(wrapper->GetDescriptor() == descriptor);
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_MulticastInlineDelegate::GetManagedType(const UDelegateFunction* signature) const
{
	FZRuntimeTypeUri uri;
	if (!FZReflectionHelper::GetUFieldRuntimeTypeLocator(signature, uri))
	{
		return {};
	}
	
	return Alc.GetType(uri);
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
	FZRuntimeTypeUri uri;
	if (!FZReflectionHelper::GetUFieldRuntimeTypeLocator(signature, uri))
	{
		return {};
	}
	
	return Alc.GetType(uri);
}


