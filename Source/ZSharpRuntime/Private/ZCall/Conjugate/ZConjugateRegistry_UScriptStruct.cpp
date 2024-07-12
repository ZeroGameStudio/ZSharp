// Copyright Zero Games. All Rights Reserved.


#include "ZCall/Conjugate/ZConjugateRegistry_UScriptStruct.h"

#include "ZSharpExportHelpers.h"
#include "ZSharpExportRuntimeSettings.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "Conjugate/ZDeclareConjugateRegistry.h"

namespace ZSharp::ZConjugateRegistry_UScriptStruct_Private
{
	static TZDeclareConjugateRegistry<FZConjugateRegistry_UScriptStruct> GDeclare;
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_UScriptStruct::Conjugate(const UScriptStruct* scriptStruct, TFunctionRef<void(const FZSelfDescriptiveScriptStruct& sdss)> initialize)
{
	const FZRuntimeTypeHandle type = GetManagedType(scriptStruct);
	FZSelfDescriptiveScriptStruct* sdss = new FZSelfDescriptiveScriptStruct { scriptStruct };
	initialize(*sdss);
	void* unmanaged = sdss->GetUnderlyingInstance();
	if (Alc.BuildConjugate(unmanaged, type))
	{
		ConjugateMap.Emplace(unmanaged, { TUniquePtr<FZSelfDescriptiveScriptStruct>(sdss), false });
		CaptureConjugate(unmanaged);

		return { unmanaged };
	}

	return {};
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_UScriptStruct::Conjugate(const UScriptStruct* scriptStruct, const void* unmanaged)
{
	const auto mutableUnmanaged = const_cast<void*>(unmanaged);
	if (const FZConjugateRec* rec = ConjugateMap.Find(mutableUnmanaged))
	{
		check(rec->ScriptStruct->GetDescriptor() == scriptStruct);
		return { mutableUnmanaged };
	}

	const FZRuntimeTypeHandle type = GetManagedType(scriptStruct);
	if (Alc.BuildConjugate(mutableUnmanaged, type))
	{
		ConjugateMap.Emplace(mutableUnmanaged, { MakeUnique<FZSelfDescriptiveScriptStruct>(scriptStruct, const_cast<void*>(unmanaged)), false });
		CaptureConjugate(mutableUnmanaged);

		return { mutableUnmanaged };
	}

	return {};
}

const ZSharp::FZSelfDescriptiveScriptStruct* ZSharp::FZConjugateRegistry_UScriptStruct::Conjugate(FZConjugateHandle handle) const
{
	const void* unmanaged = handle.Handle;
	const FZConjugateRec* rec = ConjugateMap.Find(unmanaged);
	return rec ? rec->ScriptStruct.Get() : nullptr;
}

void* ZSharp::FZConjugateRegistry_UScriptStruct::BuildConjugate(void* userdata)
{
	const UScriptStruct* scriptStruct = static_cast<UScriptStruct*>(userdata);
	auto pSdss = MakeUnique<FZSelfDescriptiveScriptStruct>(scriptStruct);
	void* unmanaged = pSdss->GetUnderlyingInstance();
	ConjugateMap.Emplace(unmanaged, { MoveTemp(pSdss), true });
	return unmanaged;
}

void ZSharp::FZConjugateRegistry_UScriptStruct::ReleaseConjugate(void* unmanaged)
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

void ZSharp::FZConjugateRegistry_UScriptStruct::GetAllConjugates(TArray<void*>& outConjugates) const
{
	for (const auto& pair : ConjugateMap)
	{
		outConjugates.Emplace(pair.Key);
	}
}

ZSharp::FZRuntimeTypeHandle ZSharp::FZConjugateRegistry_UScriptStruct::GetManagedType(const UScriptStruct* scriptStruct) const
{
	check(scriptStruct->IsNative()); // @TODO
	
	const FString moduleName = FZSharpExportHelpers::GetUFieldModuleName(scriptStruct);
	FString assemblyName;
	if (!GetDefault<UZSharpExportRuntimeSettings>()->TryGetModuleAssembly(moduleName, assemblyName))
	{
		assemblyName = ZSHARP_ENGINE_ASSEMBLY_NAME;
	}
	const FString outerExportName = FZSharpExportHelpers::GetUFieldOuterExportName(scriptStruct);
	const FString typeName = FString::Printf(TEXT("%s.%s"), *assemblyName, *outerExportName);

	FZRuntimeTypeLocatorWrapper locator;
	locator.AssemblyName = assemblyName;
	locator.TypeName = typeName;
	
	return Alc.GetType(locator);
}


