// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZExportedClass.h"
#include "Concept/ZStaticallyExportableClass.h"
#include "Trait/ZConjugateKey.h"
#include "Trait/ZConjugateRegistryId.h"
#include "Trait/ZExportedTypeName.h"
#include "Trait/ZManagedTypeInfo.h"

namespace ZSharp
{
	namespace ZStaticallyExportedClass_Private
	{
		ZSHARPEXPORT_API bool RegisterClass(IZExportedClass* cls);
	}
	
	template <CZStaticallyExportableClass T>
	class TZStaticallyExportedClass : public IZExportedClass
	{

	public:
		virtual FString GetName() const override { return TZExportedTypeName<T>::Get().Name; }
		virtual FString GetModule() const override { return TZManagedTypeInfo<T>::GetModuleName(); }
		virtual FString GetUnrealFieldPath() const override { return {}; }
		virtual uint16 GetConjugateRegistryId() const override { return TZConjugateRegistryId_V<T>; }
		virtual FString GetConjugateKey() const override { return TZConjugateKey<T>::Value; }
		virtual EZExportedClassFlags GetFlags() const override { return Flags; }
		virtual FZFullyExportedTypeName GetBaseType() const override { return {}; }

		virtual void ForeachInterface(TFunctionRef<void(const FZFullyExportedTypeName&)> action) const override
		{
			
		}
		
		virtual void ForeachMethod(TFunctionRef<void(const IZExportedMethod&)> action) const override
		{
			
		}
		
		virtual void ForeachProperty(TFunctionRef<void(const IZExportedProperty&)> action) const override
		{
			
		}

	protected:
		TZStaticallyExportedClass(bool abstract)
			: Flags(EZExportedClassFlags::Plain | (abstract ? EZExportedClassFlags::Abstract : EZExportedClassFlags::None)){}

	private:
		EZExportedClassFlags Flags;
		
	};
}


