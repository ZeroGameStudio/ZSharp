// Copyright Zero Games. All Rights Reserved.


#include "ZCall/Conjugate/ZConjugateRegistry_UScriptStruct.h"

#include "ZSharpExportHelpers.h"
#include "ZSharpExportRuntimeSettings.h"
#include "ALC/IZMasterAssemblyLoadContext.h"
#include "ZCall/ZDeclareConjugateRegistry.h"

namespace ZSharp::ZConjugateRegistry_UScriptStruct_Private
{
	static TZDeclareConjugateRegistry<FZConjugateRegistry_UScriptStruct> GDeclare;
}

ZSharp::FZConjugateHandle ZSharp::FZConjugateRegistry_UScriptStruct::Conjugate(const UScriptStruct* scriptStruct, const void* unmanaged, bool owning)
{
	const auto mutableUnmanaged = const_cast<void*>(unmanaged);
	if (const FZConjugateRec* rec = ConjugateMap.Find(mutableUnmanaged))
	{
		check(rec->ScriptStruct.Get() == scriptStruct);
		return { mutableUnmanaged };
	}

	const FZRuntimeTypeHandle type = GetManagedType(scriptStruct);
	if (Alc.BuildConjugate(mutableUnmanaged, type))
	{
		ConjugateMap.Emplace(mutableUnmanaged, { TStrongObjectPtr { const_cast<UScriptStruct*>(scriptStruct) }, const_cast<void*>(unmanaged), owning, false });
		CaptureConjugate(mutableUnmanaged);

		return { mutableUnmanaged };
	}

	return {};
}

void* ZSharp::FZConjugateRegistry_UScriptStruct::BuildConjugate(void* userdata)
{
	const UScriptStruct* scriptStruct = static_cast<UScriptStruct*>(userdata);
	auto unmanaged = FMemory::Malloc(scriptStruct->GetStructureSize(), scriptStruct->GetMinAlignment());
	scriptStruct->InitializeStruct(unmanaged);
	ConjugateMap.Emplace(unmanaged, { TStrongObjectPtr { const_cast<UScriptStruct*>(scriptStruct) }, unmanaged, true, true });
	return unmanaged;
}

void ZSharp::FZConjugateRegistry_UScriptStruct::ReleaseConjugate(void* unmanaged)
{
	const FZConjugateRec* rec = ConjugateMap.Find(unmanaged);
	if (!rec)
	{
		return;
	}

	if (rec->bOwning)
	{
		rec->ScriptStruct->DestroyStruct(unmanaged);
		FMemory::Free(unmanaged);
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

const ZSharp::FZConjugateRegistry_UScriptStruct::FZConjugateRec* ZSharp::FZConjugateRegistry_UScriptStruct::Conjugate(FZConjugateHandle handle)
{
	const void* unmanaged = handle.Handle;
	return ConjugateMap.Find(unmanaged);
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
	
	return Alc.GetType(assemblyName, typeName);
}


