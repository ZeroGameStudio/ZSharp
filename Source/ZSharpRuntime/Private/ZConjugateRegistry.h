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

		using ThisClass = FZConjugateRegistry;

	public:
		template <typename T>
		struct TZConjugateRecord
		{
			T* Unmanaged;
			bool bCaptured;
			TFunction<void(T*)> OnReleased;
		};

		template <typename T>
		using TZConjugateSubregistry = TMap<T*, TZConjugateRecord<T>>;
		
	public:
		static void Startup();
		static FZConjugateRegistry* Get();

	public:
		explicit FZConjugateRegistry(IZMasterAssemblyLoadContext* masterALC);
		~FZConjugateRegistry();

	public:
		FZConjugateHandle Conjugate(UObject* unmanaged);
		void ReleaseConjugate(UObject* unmanaged);
		
		FZConjugateHandle Conjugate(FString* unmanaged, const TFunction<void(FString*)>& onReleased = nullptr);
		void Conjugate(FString* unmanaged, FZConjugateHandle managed, const TFunction<void(FString*)>& onReleased = nullptr);
		void ReleaseConjugate(FString* unmanaged);

		FZConjugateHandle Conjugate(FZTypedScriptStruct* unmanaged, const TFunction<void(FZTypedScriptStruct*)>& onReleased = nullptr);
        void Conjugate(FZTypedScriptStruct* unmanaged, FZConjugateHandle managed, const TFunction<void(FZTypedScriptStruct*)>& onReleased = nullptr);
        void ReleaseConjugate(FZTypedScriptStruct* unmanaged);

		void* Conjugate(FZConjugateHandle managed) const;

	public:
		template <typename T>
		T* Conjugate(FZConjugateHandle managed) const { return StaticCast<T*>(Conjugate(managed)); }

	private:
		static void HandleMasterALCLoaded(IZMasterAssemblyLoadContext* alc);
		static void HandleMasterALCUnloaded();

	private:
		void ReleaseConjugates(bool bCapturedOnly);
		void GuardRelease();
		
		bool IsGuarded() const { return !!ZCallToManagedDepth; }
		void CheckGuarded() const;
		void CheckNoGuarded() const;
		
	private:
		void HandlePreZCallToManaged();
		void HandlePostZCallToManaged();
		void HandleGarbageCollectComplete();

	private:
		static TUniquePtr<FZConjugateRegistry> GSingleton;

	private:
		IZMasterAssemblyLoadContext* MasterALC;

	private:
		// Registries
		//  - UObject
		TMap<FObjectKey, UObject*> ObjectRegistry;

		//  - Captureables
		TZConjugateSubregistry<FString> StringRegistry;

		// Guard
		uint32 ZCallToManagedDepth;
		
	};
}


