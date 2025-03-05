// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZStrangeConjugateRegistryBase.h"
#include "Interop/ZRuntimeTypeHandle.h"

namespace ZSharp
{
	class ZSHARPRUNTIME_API FZConjugateRegistry_Array : public TZStrangeConjugateRegistryBase<FZConjugateRegistry_Array, FZSelfDescriptiveScriptArray, true>
	{

		friend Super;
		friend TZStrangeConjugateRegistryBase;

	public:
		FZConjugateRegistry_Array(IZMasterAssemblyLoadContext& alc) : TZStrangeConjugateRegistryBase(alc){}

	private:
		virtual FString GetReferencerName() const override { return "ZConjugateRegistry_Array"; }

	private:
		static FZSelfDescriptiveScriptArray* BuildConjugateWrapper(void* userdata);
		static void ValidateConjugateWrapper(const FProperty* elementProperty, const FZSelfDescriptiveScriptArray* wrapper);

	private:
		FZRuntimeTypeHandle GetManagedType(const FProperty* elementProperty) const;

	};

	class ZSHARPRUNTIME_API FZConjugateRegistry_Set : public TZStrangeConjugateRegistryBase<FZConjugateRegistry_Set, FZSelfDescriptiveScriptSet, true>
	{

		friend Super;
		friend TZStrangeConjugateRegistryBase;

	public:
		FZConjugateRegistry_Set(IZMasterAssemblyLoadContext& alc) : TZStrangeConjugateRegistryBase(alc){}

	private:
		virtual FString GetReferencerName() const override { return "ZConjugateRegistry_Set"; }
		
	private:
		static FZSelfDescriptiveScriptSet* BuildConjugateWrapper(void* userdata);
		static void ValidateConjugateWrapper(const FProperty* elementProperty, const FZSelfDescriptiveScriptSet* wrapper);

	private:
		FZRuntimeTypeHandle GetManagedType(const FProperty* elementProperty) const;
	
	};

	class ZSHARPRUNTIME_API FZConjugateRegistry_Map : public TZStrangeConjugateRegistryBase<FZConjugateRegistry_Map, FZSelfDescriptiveScriptMap, true>
	{

		friend Super;
		friend TZStrangeConjugateRegistryBase;

	public:
		FZConjugateRegistry_Map(IZMasterAssemblyLoadContext& alc) : TZStrangeConjugateRegistryBase(alc){}

	private:
		virtual FString GetReferencerName() const override { return "ZConjugateRegistry_Map"; }
		
	private:
		static FZSelfDescriptiveScriptMap* BuildConjugateWrapper(void* userdata);
		static void ValidateConjugateWrapper(const TPair<const FProperty*, const FProperty*>* descriptor, const FZSelfDescriptiveScriptMap* wrapper);

	private:
		FZRuntimeTypeHandle GetManagedType(const TPair<const FProperty*, const FProperty*>* descriptor) const;
		
	};

	class ZSHARPRUNTIME_API FZConjugateRegistry_Optional : public TZStrangeConjugateRegistryBase<FZConjugateRegistry_Optional, FZSelfDescriptiveOptional, true>
	{

		friend Super;
		friend TZStrangeConjugateRegistryBase;

	public:
		FZConjugateRegistry_Optional(IZMasterAssemblyLoadContext& alc) : TZStrangeConjugateRegistryBase(alc){}

	private:
		virtual FString GetReferencerName() const override { return "ZConjugateRegistry_Optional"; }
		
	private:
		static FZSelfDescriptiveOptional* BuildConjugateWrapper(void* userdata);
		static void ValidateConjugateWrapper(const FProperty* elementProperty, const FZSelfDescriptiveOptional* wrapper);

	private:
		FZRuntimeTypeHandle GetManagedType(const FProperty* elementProperty) const;
	
	};

	class ZSHARPRUNTIME_API FZConjugateRegistry_UScriptStruct : public TZStrangeConjugateRegistryBase<FZConjugateRegistry_UScriptStruct, FZSelfDescriptiveScriptStruct, true>
	{

		friend Super;
		friend TZStrangeConjugateRegistryBase;

	public:
		FZConjugateRegistry_UScriptStruct(IZMasterAssemblyLoadContext& alc) : TZStrangeConjugateRegistryBase(alc){}

	private:
		virtual FString GetReferencerName() const override { return "ZConjugateRegistry_UScriptStruct"; }
		
	private:
		static FZSelfDescriptiveScriptStruct* BuildConjugateWrapper(void* userdata);
		static void ValidateConjugateWrapper(const UScriptStruct* descriptor, const FZSelfDescriptiveScriptStruct* wrapper);

	private:
		FZRuntimeTypeHandle GetManagedType(const UScriptStruct* descriptor) const;
	
	};

	class ZSHARPRUNTIME_API FZConjugateRegistry_Delegate : public TZStrangeConjugateRegistryBase<FZConjugateRegistry_Delegate, FZSelfDescriptiveScriptDelegate>
	{

		friend Super;
		friend TZStrangeConjugateRegistryBase;

	public:
		FZConjugateRegistry_Delegate(IZMasterAssemblyLoadContext& alc) : TZStrangeConjugateRegistryBase(alc){}

	private:
		static FZSelfDescriptiveScriptDelegate* BuildConjugateWrapper(void* userdata);
		static void ValidateConjugateWrapper(const UDelegateFunction* descriptor, const FZSelfDescriptiveScriptDelegate* wrapper);

	private:
		FZRuntimeTypeHandle GetManagedType(const UDelegateFunction* descriptor) const;
	
	};

	class ZSHARPRUNTIME_API FZConjugateRegistry_MulticastInlineDelegate : public TZStrangeConjugateRegistryBase<FZConjugateRegistry_MulticastInlineDelegate, FZSelfDescriptiveMulticastInlineScriptDelegate>
	{

		friend Super;
		friend TZStrangeConjugateRegistryBase;

	public:
		FZConjugateRegistry_MulticastInlineDelegate(IZMasterAssemblyLoadContext& alc) : TZStrangeConjugateRegistryBase(alc){}

	private:
		static FZSelfDescriptiveMulticastInlineScriptDelegate* BuildConjugateWrapper(void* userdata);
		static void ValidateConjugateWrapper(const UDelegateFunction* descriptor, const FZSelfDescriptiveMulticastInlineScriptDelegate* wrapper);

	private:
		FZRuntimeTypeHandle GetManagedType(const UDelegateFunction* descriptor) const;
	
	};

	class ZSHARPRUNTIME_API FZConjugateRegistry_MulticastSparseDelegate : public TZStrangeConjugateRegistryBase<FZConjugateRegistry_MulticastSparseDelegate, FZSelfDescriptiveMulticastSparseScriptDelegate>
	{

		friend Super;
		friend TZStrangeConjugateRegistryBase;

	public:
		FZConjugateRegistry_MulticastSparseDelegate(IZMasterAssemblyLoadContext& alc) : TZStrangeConjugateRegistryBase(alc){}

	private:
		static FZSelfDescriptiveMulticastSparseScriptDelegate* BuildConjugateWrapper(void* userdata);
		static void ValidateConjugateWrapper(const USparseDelegateFunction* descriptor, const FZSelfDescriptiveMulticastSparseScriptDelegate* wrapper);

	private:
		FZRuntimeTypeHandle GetManagedType(const USparseDelegateFunction* descriptor) const;
	
	};
}


