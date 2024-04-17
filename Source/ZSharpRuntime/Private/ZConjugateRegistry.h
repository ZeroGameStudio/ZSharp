// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Interop/ZConjugateHandle.h"

namespace ZSharp
{
	class IZMasterAssemblyLoadContext;
	
	struct FZTypedScriptStruct
	{
		UScriptStruct* ScriptStruct;
		void* Memory;
	};
	
	class FZConjugateRegistry
	{
		
	public:
		static FZConjugateRegistry& Get();

	public:
		FZConjugateRegistry();

	public:
		FZConjugateHandle BuildConjugate(UObject* unmanaged);
		void ReleaseConjugate(UObject* unmanaged);
		
		FZConjugateHandle BuildConjugate(FString* unmanaged);
		void BuildConjugate(FString* unmanaged, FZConjugateHandle managed);
		void ReleaseConjugate(FString* unmanaged);

		FZConjugateHandle BuildConjugate(FZTypedScriptStruct* unmanaged);
        void BuildConjugate(FZTypedScriptStruct* unmanaged, FZConjugateHandle managed);
        void ReleaseConjugate(FZTypedScriptStruct* unmanaged);

	public:
		template <typename T>
		T* Conjugate(FZConjugateHandle managed) const { return (T*)InternalConjugate(managed); }

		// template <typename T>
		// FZConjugateHandle Conjugate(T* unmanaged) const { return InternalConjugate(unmanaged); }

	private:
		FZConjugateHandle InternalConjugate(void* unmanaged) const;
		void* InternalConjugate(FZConjugateHandle managed) const;
	
	private:
		void HandleGarbageCollectComplete();
		void HandleMasterALCLoaded(IZMasterAssemblyLoadContext* alc);
		void HandleMasterALCUnloaded();

	private:
		TMap<FObjectKey, UObject*> ObjectRegistry;
		TSet<FString*> StringRegistry;
		
	};
}


