// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZObjectWrapperConjugateRegistryBase.h"
#include "Reflection/Wrapper/ZSelfDescriptiveObjectWrappers.h"

namespace ZSharp
{
	class ZSHARPRUNTIME_API FZConjugateRegistry_SubclassOf : public TZObjectWrapperConjugateRegistryBase<FZSelfDescriptiveSubclassOf>
	{
	public:
		explicit FZConjugateRegistry_SubclassOf(IZMasterAssemblyLoadContext& alc) : TZObjectWrapperConjugateRegistryBase(alc){}
	};
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_SoftClassPtr : public TZObjectWrapperConjugateRegistryBase<FZSelfDescriptiveSoftClassPtr>
	{
	public:
		explicit FZConjugateRegistry_SoftClassPtr(IZMasterAssemblyLoadContext& alc) : TZObjectWrapperConjugateRegistryBase(alc){}
	};
	
	class ZSHARPRUNTIME_API FZConjugateRegistry_SoftObjectPtr : public TZObjectWrapperConjugateRegistryBase<FZSelfDescriptiveSoftObjectPtr>
	{
	public:
		explicit FZConjugateRegistry_SoftObjectPtr(IZMasterAssemblyLoadContext& alc) : TZObjectWrapperConjugateRegistryBase(alc){}
	};

	class ZSHARPRUNTIME_API FZConjugateRegistry_WeakObjectPtr : public TZObjectWrapperConjugateRegistryBase<FZSelfDescriptiveWeakObjectPtr>
	{
	public:
		explicit FZConjugateRegistry_WeakObjectPtr(IZMasterAssemblyLoadContext& alc) : TZObjectWrapperConjugateRegistryBase(alc){}
	};

	class ZSHARPRUNTIME_API FZConjugateRegistry_LazyObjectPtr : public TZObjectWrapperConjugateRegistryBase<FZSelfDescriptiveLazyObjectPtr>
	{
	public:
		explicit FZConjugateRegistry_LazyObjectPtr(IZMasterAssemblyLoadContext& alc) : TZObjectWrapperConjugateRegistryBase(alc){}
	};

	class ZSHARPRUNTIME_API FZConjugateRegistry_ScriptInterface : public TZObjectWrapperConjugateRegistryBase<FZSelfDescriptiveScriptInterface>
	{
	public:
		explicit FZConjugateRegistry_ScriptInterface(IZMasterAssemblyLoadContext& alc) : TZObjectWrapperConjugateRegistryBase(alc){}
	};

	class ZSHARPRUNTIME_API FZConjugateRegistry_StrongObjectPtr : public TZObjectWrapperConjugateRegistryBase<FZSelfDescriptiveStrongObjectPtr>
	{
	public:
		explicit FZConjugateRegistry_StrongObjectPtr(IZMasterAssemblyLoadContext& alc) : TZObjectWrapperConjugateRegistryBase(alc){}
	};
}


