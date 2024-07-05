// Copyright Zero Games. All Rights Reserved.


#include "ZCall/Conjugate/ZConjugateRegistry_Array.h"

#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ZCall/ZDeclareConjugateRegistry.h"
#include "ZCall/Property/ZPropertyFactory.h"

namespace ZSharp::ZConjugateRegistry_Array_Private
{
	static TZDeclareConjugateRegistry<FZConjugateRegistry_Array> GDeclare;
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_Array::Conjugate(const FProperty* elementProperty, const FScriptArray* unmanaged, bool owning)
{
	const auto mutableUnmanaged = const_cast<FScriptArray*>(unmanaged);
	if (const FZConjugateRec* rec = ConjugateMap.Find(mutableUnmanaged))
	{
		check(rec->Array->GetElementProperty() == elementProperty);
		return { mutableUnmanaged };
	}

	const FZRuntimeTypeHandle type = GetManagedType(elementProperty);
	if (Alc.BuildConjugate(mutableUnmanaged, type))
	{
		ConjugateMap.Emplace(mutableUnmanaged, { MakeUnique<FZSelfDescriptiveScriptArray>(elementProperty, const_cast<FScriptArray*>(unmanaged), owning), false });
		CaptureConjugate(mutableUnmanaged);

		return { mutableUnmanaged };
	}

	return {};
}

ZSharp::FZSelfDescriptiveScriptArray* ZSharp::FZConjugateRegistry_Array::Conjugate(FZConjugateHandle handle) const
{
	const void* unmanaged = handle.Handle;
	const FZConjugateRec* rec = ConjugateMap.Find(unmanaged);
	return rec ? rec->Array.Get() : nullptr;
}

void* ZSharp::FZConjugateRegistry_Array::BuildConjugate(void* userdata)
{
	struct
	{
		FZPropertyDesc Desc;
	} typedUserdata = *static_cast<decltype(typedUserdata)*>(userdata);
	
	auto pSdsa = MakeUnique<FZSelfDescriptiveScriptArray>(FZPropertyFactory::Create(typedUserdata.Desc));
	void* unmanaged = pSdsa->GetUnderlyingInstance();
	ConjugateMap.Emplace(unmanaged, { MoveTemp(pSdsa), true });
	return unmanaged;
}

void ZSharp::FZConjugateRegistry_Array::ReleaseConjugate(void* unmanaged)
{
	const FZConjugateRec* rec = ConjugateMap.Find(unmanaged);
	if (!rec)
	{
		return;
	}
	
	if (!rec->bBlack)
	{
		Alc.ReleaseConjugate(unmanaged);
	}

	ConjugateMap.Remove(unmanaged);
}

void ZSharp::FZConjugateRegistry_Array::GetAllConjugates(TArray<void*>& outConjugates) const
{
	for (const auto& pair : ConjugateMap)
	{
		outConjugates.Emplace(pair.Key);
	}
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_Array::GetManagedType(const FProperty* elementProperty) const
{
	checkNoEntry();
	return {};
	// check(scriptStruct->IsNative()); // @TODO
	//
	// const FString moduleName = FZSharpExportHelpers::GetUFieldModuleName(scriptStruct);
	// FString assemblyName;
	// if (!GetDefault<UZSharpExportRuntimeSettings>()->TryGetModuleAssembly(moduleName, assemblyName))
	// {
	// 	assemblyName = ZSHARP_ENGINE_ASSEMBLY_NAME;
	// }
	// const FString outerExportName = FZSharpExportHelpers::GetUFieldOuterExportName(scriptStruct);
	// const FString typeName = FString::Printf(TEXT("%s.%s"), *assemblyName, *outerExportName);
	//
	// return Alc.GetType(assemblyName, typeName);
}
