// Copyright Zero Games. All Rights Reserved.


#include "Conjugate/ZConjugateRegistry_UScriptStruct.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ALC/ZRuntimeTypeUri.h"
#include "Conjugate/ZDeclareConjugateRegistryMacros.h"
#include "Reflection/ZReflectionHelper.h"
#include "Reflection/Wrapper/ZSelfDescriptiveScriptStruct.h"

ZSHARP_DECLARE_CONJUGATE_REGISTRY(FZConjugateRegistry_UScriptStruct)

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


